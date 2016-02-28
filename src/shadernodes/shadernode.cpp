#include "shadernode.h"
#include "shaderbuilder.h"
#include "outputnode.h"

#include <QDebug>
#include <QMetaProperty>
#include <QRegularExpression>
#include <QSignalMapper>

QString randomName() {
    QString letters = "abcdefghijklmnopqrstuvwzyz";
    QString result;
    result.resize(4);
    for(int i = 0; i < result.length(); i++) {
        result[i] = letters.at(qrand() % letters.length());
    }
    return result;
}

ShaderNode::ShaderNode(QObject *parent)
    : QObject(parent)
{
    setName(randomName());
}

QString ShaderNode::name() const
{
    return m_name;
}

QString ShaderNode::type() const
{
    return m_type;
}

QString ShaderNode::result() const
{
    return m_result;
}

QString ShaderNode::header() const
{
    return m_header;
}

QQmlListProperty<VariantShaderNode> ShaderNode::variantNodes()
{
    return QQmlListProperty<VariantShaderNode>(this, m_variantNodes);
}

QString ShaderNode::identifier() const
{
    return m_identifier;
}

QString ShaderNode::generateHeader() const
{
    if(m_hasGeneratedHeader) {
        return QString();
    }

    QString headerResult = "";
    for(const ShaderNode *node : m_dependencies) {
        headerResult += node->generateHeader();
    }
    headerResult += m_header;
    m_hasGeneratedHeader = true;
    return headerResult;
}

QString ShaderNode::convert(QString targetType, QString identifier) const
{
    QString v;
    if(!identifier.isEmpty()) {
        v = identifier;
    } else {
        v = m_identifier;
    }
    if(targetType.isEmpty() || m_type == targetType) {
        return v;
    }

    QVariantMap scalar{
        {"bool", "bool(" + v + ")"},
        {"int", "int(" + v + ")"},
        {"uint", "uint(" + v + ")"},
        {"float", "float(" + v + ")"},
        {"double", "double(" + v + ")"},
        {"vec3", "vec2(" + v + ", 0.0)"},
        {"vec3", "vec3(" + v + ", 0.0, 0.0)"},
        {"vec4", "vec4(" + v + ", 0.0, 0.0, 1.0)"}
    };

    QVariantMap conversions{
        {"bool", scalar},
        {"int", scalar},
        {"uint", scalar},
        {"float", scalar},
        {"double", scalar},
        {"vec2", QVariantMap{
                {"float", "0.5 * " + v + ".x + " + v + ".y)"},
                {"vec3", "vec3(" + v + ", 0.0)"},
                {"vec4", "vec4(" + v + ", 0.0, 1.0)"}
            }
        },
        {"vec3", QVariantMap{
                {"float", "1.0 / 3.0 * (" + v + ".x + " + v + ".y + " + v + ".z)"},
                {"vec2", v + ".xy"},
                {"vec4", "vec4(" + v + ", 1.0)"}
            }
        },
        {"vec4", QVariantMap{
                {"float", "0.25 * (" + v + ".x + " + v + ".y + " + v + ".z + " + v + ".w)"},
                {"vec2", v + ".xy"},
                {"vec3", v + ".xyz"}
            }
        }
    };
    if(conversions.contains(m_type)) {
        QVariantMap typeConversions = conversions[m_type].toMap();
        if(typeConversions.contains(targetType)) {
            return "(" + typeConversions[targetType].toString() + ")";
        }
    }
    qWarning() << "WARNING: No known conversion from " << m_type << " to " << targetType;
    return v;
}

void ShaderNode::setup(ShaderBuilder* shaderBuilder)
{
    if(m_hasSetup) {
        return;
    }

    qDebug() << "Working on" << name();

    QString sourceContent;
    sourceContent = m_source;
    if(!m_result.isEmpty()) {
        sourceContent += m_identifier + " = " + m_result + ";\n";
    }
    qDebug() << "Source was" << sourceContent;

    if(!sourceContent.isEmpty()) {
        QRegularExpression propertyRegex("\\$(?:\\(\\s*)?([a-z0-9]+)\\s*\\)?(?:\\s*,\\s*([a-z0-9]+)\\s*\\))?");
        QRegularExpressionMatchIterator matches = propertyRegex.globalMatch(sourceContent);
        while(matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            QString propertyName = match.captured(1);
            QString targetType;
            if(match.lastCapturedIndex() > 1) {
                targetType = match.captured(2);
            }

            int propertyIndex = metaObject()->indexOfProperty(propertyName.toStdString().c_str());
            if(propertyIndex < 0) {
                qWarning() << "ShaderNode: Could not find property with name" << propertyName << "in" << this << "object with name" << name();
                continue;
            }
            QMetaProperty metaProperty = metaObject()->property(propertyIndex);
            QVariant value = metaProperty.read(this);
            ShaderNode *node = qvariant_cast<ShaderNode*>(value);
            OutputNode *outputNode = qvariant_cast<OutputNode*>(value);
            QString identifier;
            if(outputNode) {
                // If it is an output node, we need to depend on its parent too
                ShaderNode *parentNode = qobject_cast<ShaderNode*>(outputNode->parent());
                if(parentNode && parentNode != this) {
                    parentNode->setup(shaderBuilder);
                    if(!m_dependencies.contains(parentNode)) {
                        m_dependencies.append(parentNode);
                    }
                }
            }
            if(node) {
                node->setup(shaderBuilder);
                if(!m_dependencies.contains(node)) {
                    m_dependencies.append(node);
                }
                identifier = node->convert(targetType);
            } else {
                identifier = propertyName + "_" + randomName(); // TODO add conversion if necessary

                if(!metaProperty.hasNotifySignal()) {
                    qWarning() << "ShaderNode: property" << propertyName << "has no notification signal in" << this << "object with name" << name();
                }
                shaderBuilder->addUniform(this, propertyName, identifier, value, metaProperty);
            }
            sourceContent.replace(QRegularExpression("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[a-z0-9]+\\s*\\))?"), identifier);
        }
    }
    qDebug() << "Source became" << sourceContent;
    m_resolvedSource = sourceContent;
    m_hasSetup = true;
}

QString ShaderNode::generateBody() const
{
    if(m_hasGeneratedBody) {
        return QString();
    }
    if(m_type.isEmpty()) {
        qWarning() << "WARNING: ShaderNode" << m_name << m_identifier << "is missing type.";
        return QString();
    }
    QString body = "";
    for(ShaderNode* dependency : m_dependencies) {
        body += dependency->generateBody();
    }
    body += m_type + " " + m_identifier + ";\n";
    if(!m_resolvedSource.isEmpty()) {
        body += m_resolvedSource + "\n";
    }
    body += "\n";
    m_hasGeneratedBody = true;
    return body;
}

void ShaderNode::reset() const
{
    for(ShaderNode* dependency : m_dependencies) {
        dependency->reset();
    }
    m_hasGeneratedBody = false;
    m_hasGeneratedHeader = false;
    m_hasSetup = false;
}

void ShaderNode::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    m_identifier = name + "_" + randomName();

    emit identifierChanged(m_identifier);
    emit nameChanged(name);
}

void ShaderNode::setType(QString type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}

void ShaderNode::setResult(QString result)
{
    if (m_result == result)
        return;

    m_result = result;
    emit resultChanged(result);
}

void ShaderNode::setHeader(QString header)
{
    if (m_header == header)
        return;

    m_header = header;
    emit headerChanged(header);
}

void ShaderNode::setSource(QString source)
{
    if (m_source == source)
            return;

        m_source = source;
        emit sourceChanged(source);
}

ShaderBuilder *ShaderNode::shaderBuilder() const
{
    return m_shaderBuilder;
}

void ShaderNode::setShaderBuilder(ShaderBuilder *shaderBuilder)
{
    for(ShaderNode *node : m_dependencies) {
        node->setShaderBuilder(shaderBuilder);
    }
    if(m_shaderBuilder) {
        disconnect(this, 0, m_shaderBuilder, SLOT(triggerOutputChange()));
    }
    m_shaderBuilder = shaderBuilder;
    if(!m_shaderBuilder) {
        return;
    }
    connect(this, &ShaderNode::headerChanged, m_shaderBuilder, &ShaderBuilder::triggerOutputChange);
    connect(this, &ShaderNode::resultChanged, m_shaderBuilder, &ShaderBuilder::triggerOutputChange);
    connect(this, &ShaderNode::sourceChanged, m_shaderBuilder, &ShaderBuilder::triggerOutputChange);
    connect(this, &ShaderNode::typeChanged, m_shaderBuilder, &ShaderBuilder::triggerOutputChange);
    connect(this, &ShaderNode::identifierChanged, m_shaderBuilder, &ShaderBuilder::triggerOutputChange);
}

QString ShaderNode::source() const
{
    return m_source;
}
