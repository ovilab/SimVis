#include "shadernode.h"
#include "shaderbuilder.h"
#include "shaderutils.h"
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

QString ShaderNode::convert(const QString &targetType, const QString &identifier) const
{
    QString v = identifier;
    if(v.isEmpty()) {
        v = m_identifier;
    }
    return ShaderUtils::convert(type(), targetType, v);
}

bool ShaderNode::setup(ShaderBuilder* shaderBuilder)
{
    if(m_hasSetup) {
        return true;
    }
    if(!m_requirement) {
        qWarning() << "ShaderNode::setup(): Requirement for" << this << name() << "is not satisfied.";
        return false;
    }

    QString sourceContent;
    sourceContent = m_source;
    if(!m_result.isEmpty()) {
        sourceContent += m_identifier + " = " + m_result + ";\n";
    }

    bool success = true;
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
            QString targetIdentifier;
            QString sourceType;
            if(outputNode) {
                // If it is an output node, we need to depend on its parent too
                ShaderNode *parentNode = qobject_cast<ShaderNode*>(outputNode->parent());
                if(parentNode && parentNode != this) {
                    success = success && parentNode->setup(shaderBuilder);
                    if(!m_dependencies.contains(parentNode)) {
                        m_dependencies.append(parentNode);
                    }
                }
            }
            if(node) {
                success = success && node->setup(shaderBuilder);
                if(!m_dependencies.contains(node)) {
                    m_dependencies.append(node);
                }
                targetIdentifier = node->identifier();
                sourceType = node->type();
            } else {
                targetIdentifier = propertyName + "_" + randomName();
                sourceType = glslType(value);

                if(!metaProperty.hasNotifySignal()) {
                    qWarning() << "ShaderNode: property" << propertyName << "has no notification signal in" << this << "object with name" << name();
                }
                shaderBuilder->addUniform(this, propertyName, targetIdentifier, value, metaProperty);
            }
            sourceContent.replace(QRegularExpression("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[a-z0-9]+\\s*\\))?"),
                                  ShaderUtils::convert(sourceType, targetType, targetIdentifier));
        }
    }
    m_resolvedSource = sourceContent;
    m_hasSetup = true;
    return success;
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

void ShaderNode::setRequirement(bool requirement)
{
    if (m_requirement == requirement)
            return;

        m_requirement = requirement;
        emit requirementChanged(requirement);
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

QString ShaderNode::glslType(QVariant value) const
{
    return ShaderUtils::glslType(value);
}

QString ShaderNode::source() const
{
    return m_source;
}

bool ShaderNode::requirement() const
{
    return m_requirement;
}
