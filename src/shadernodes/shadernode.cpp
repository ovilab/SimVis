#include "shadernode.h"
#include "variantshadernode.h"
#include "shaderbuilder.h"

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

QString ShaderNode::initialization() const
{
    return m_initialization;
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

QString ShaderNode::resolve(ShaderNode *node, QString targetType)
{
    if(!node) {
        qWarning() << "ERROR: Cannot resolve nullptr node.";
        return QString();
    }
    VariantShaderNode *variantShaderNode = qobject_cast<VariantShaderNode*>(node);
    if(variantShaderNode) {
        ShaderNode *variantNodeValue = qvariant_cast<ShaderNode*>(variantShaderNode->value());
        if(variantNodeValue) { // if a variant node carries a ShaderNode, it's just a wrapper
            node = variantNodeValue;
        }
    }
    m_dependencies.append(node);
    if(targetType.isEmpty()) {
        return node->identifier();
    } else {
        return node->convert(targetType);
    }
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

QList<VariantShaderNode*> ShaderNode::uniformDependencies() const
{
    QList<VariantShaderNode*> uniforms;
    for(ShaderNode* node : m_dependencies) {
        uniforms.append(node->uniformDependencies());
        VariantShaderNode *variantNode = qobject_cast<VariantShaderNode*>(node);
        if(variantNode) {
            uniforms.append(variantNode);
        }
    }
    return uniforms;
}

void ShaderNode::setup(ShaderBuilder* shaderBuilder)
{
    if(m_hasSetup) {
        return;
    }
    qDebug() << "Setting up" << name() << result();
    m_resolvedResult = m_result;

    if(!m_result.isEmpty()) {
        QRegularExpression propertyRegex("\\$([a-z0-9]+)(?:\\.([a-z0-9]+))?");
        QRegularExpressionMatchIterator matches = propertyRegex.globalMatch(m_result);
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
            QString identifier;
            if(node) {
                node->setup(shaderBuilder);
                m_dependencies.append(node);
                identifier = node->convert(targetType);
            } else {
                identifier = propertyName + "_" + randomName(); // TODO add conversion if necessary

                if(!metaProperty.hasNotifySignal()) {
                    qWarning() << "ShaderNode: property" << propertyName << "has no notification signal in" << this << "object with name" << name();
                }
                QString type = glslTypeFromVariant(value);
                shaderBuilder->addUniform(this, propertyName, identifier, value, type, metaProperty);
            }
            m_resolvedResult.replace(QRegularExpression("\\$" + propertyName + "(\\.[a-z0-9]+)?"), identifier);
        }
    }
    qDebug() << "Resulted in" << m_resolvedResult;
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
    if(!m_initialization.isEmpty()) {
        body += m_initialization + ";\n";
    }
    if(!m_resolvedResult.isEmpty()) {
        body += m_identifier + " = " + m_resolvedResult + ";\n";
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

void ShaderNode::setInitialization(QString initialization)
{
    if (m_initialization == initialization)
        return;

    m_initialization = initialization;
    emit initializationChanged(initialization);
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
        disconnect(this, 0, m_shaderBuilder, SLOT(receiveOutputChange()));
    }
    m_shaderBuilder = shaderBuilder;
    if(!m_shaderBuilder) {
        return;
    }
    connect(this, &ShaderNode::headerChanged, m_shaderBuilder, &ShaderBuilder::receiveOutputChange);
    connect(this, &ShaderNode::resultChanged, m_shaderBuilder, &ShaderBuilder::receiveOutputChange);
    connect(this, &ShaderNode::identifierChanged, m_shaderBuilder, &ShaderBuilder::receiveOutputChange);
    connect(this, &ShaderNode::initializationChanged, m_shaderBuilder, &ShaderBuilder::receiveOutputChange);
    connect(this, &ShaderNode::typeChanged, m_shaderBuilder, &ShaderBuilder::receiveOutputChange);
}

QString ShaderNode::glslTypeFromVariant(QVariant value) const
{
    switch(value.type()) {
    case QVariant::Bool:
        return QString("bool");
        break;
    case QVariant::Int:
        return QString("int");
        break;
    case QVariant::Double:
        return QString("float");
        break;
    case QVariant::Vector2D:
        return QString("vec2");
        break;
    case QVariant::Vector3D:
        return QString("vec3");
        break;
    case QVariant::Vector4D:
        return QString("vec4");
        break;
    default:
        qWarning() << "WARNING: GlslVariantBridge could not identify type" << value.typeName();
        return QString("float");
        break;
    }
}
