#include "shadernode.h"
#include "shaderbuilder.h"
#include "shaderutils.h"
#include "shadergroup.h"

#include <QDebug>
#include <QJSValueIterator>
#include <QMetaProperty>
#include <QRegularExpression>
#include <QSignalMapper>
#include <QQmlFile>
#include <QFile>

ShaderNode::ShaderNode(QObject *parent)
    : QObject(parent)
{
    setName("unnamed");
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
    if(!m_header.isEmpty()) {
        QString headerIdentifier = "SHADER_NODE_HEADER_" + name().toUpper();
        headerResult += "#ifndef " + headerIdentifier + "\n";
        headerResult += "#define " + headerIdentifier + "\n";
        headerResult += m_header + "\n";
        headerResult += "#endif // " + headerIdentifier + "\n";
    }
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

    m_dependencies.clear();

    bool success = true;
    if(!sourceContent.isEmpty()) {
        // matches '$property' or '$(property, type)'
        QRegularExpression propertyRegex("\\$(?:\\(\\s*)?([_a-zA-Z0-9]+)\\s*\\)?(?:\\s*,\\s*([_a-zA-Z0-9]+)\\s*\\))?");
        QRegularExpressionMatchIterator matches = propertyRegex.globalMatch(sourceContent);
        QList<QString> alreadyReplaced;
        while(matches.hasNext()) {
            QRegularExpressionMatch match = matches.next();
            QString propertyName = match.captured(1);
            // GLSL doesn't allow double underscores. Use name without underscores for identifiers.
            QString propertyNameNoUnderscores = QString(propertyName).replace("_", "");
            if(alreadyReplaced.contains(propertyName)) {
                continue;
            }
            QString targetType;
            if(match.lastCapturedIndex() > 1) {
                targetType = match.captured(2);
            }
            if(propertyName == "this") {
                // $this is a special keyword that refers to the node itself, it cannot be used for a property name
                QString thisIdentifier = identifier();
                QRegularExpression namedRegex("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[_a-zA-Z0-9]+\\s*\\))?");
                sourceContent.replace(namedRegex, thisIdentifier);
                alreadyReplaced.append(propertyName);
                continue;
            }
            QByteArray propertyNameArray = propertyName.toUtf8();
            int propertyIndex = metaObject()->indexOfProperty(propertyNameArray.constData());
            if(propertyIndex < 0) {
                // No connected property, assume internal variable that just needs a unique name
                QString propertylessIdentifier = propertyNameNoUnderscores + "_" + ShaderUtils::generateName();
                QRegularExpression namedRegex("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[_a-zA-Z0-9]+\\s*\\))?");
                sourceContent.replace(namedRegex, propertylessIdentifier);
                alreadyReplaced.append(propertyName);
                continue;
            }
            QMetaProperty metaProperty = metaObject()->property(propertyIndex);
            QVariant value = metaProperty.read(this);

            ShaderGroup *shaderGroup = qvariant_cast<ShaderGroup*>(value);
            if(shaderGroup) {
                int i = 0;
                for(ShaderNode *groupNode : shaderGroup->m_nodes) {
                    success = success && groupNode->setup(shaderBuilder);
                    if(!m_dependencies.contains(groupNode)) {
                        m_dependencies.append(groupNode);
                    }
                    QString targetIdentifier = groupNode->identifier();
                    QString sourceType = groupNode->type();
                    // replaces '$property' or '$(property, type)'
                    QRegularExpression indexedRegex("\\$(\\(\\s*)?" + propertyName + "\\[" + QString::number(i) +"\\](\\s*,\\s*[_a-zA-Z0-9]+\\s*\\))?");
                    sourceContent.replace(indexedRegex, ShaderUtils::convert(sourceType, targetType, targetIdentifier));
                    i++;
                }
                continue;
            }

            ShaderNode *node = qvariant_cast<ShaderNode*>(value);

            QString targetIdentifier;
            QString sourceType;
            if(node) {
                // Take over any declared dependencies
                for(ShaderNode* declaredDependency : node->m_declaredDependencies) {
                    if(declaredDependency && declaredDependency != this) {
                        success = success && declaredDependency->setup(shaderBuilder);
                        if(!m_dependencies.contains(declaredDependency)) {
                            m_dependencies.append(declaredDependency);
                        }
                    }
                }
                success = success && node->setup(shaderBuilder);
                if(!m_dependencies.contains(node)) {
                    m_dependencies.append(node);
                }
                targetIdentifier = node->identifier();
                sourceType = node->type();
            } else {
                // make a uniform
                QString uniformPrefix = "uniform";
                switch(shaderBuilder->shaderType()) {
                case ShaderBuilder::ShaderType::Fragment:
                    uniformPrefix = "fragment_uniform";
                    break;
                case ShaderBuilder::ShaderType::Geometry:
                    uniformPrefix = "geometry_uniform";
                    break;
                case ShaderBuilder::ShaderType::Vertex:
                    uniformPrefix = "vertex_uniform";
                    break;
                default:
                    break;
                }

                targetIdentifier = uniformPrefix + "_" + propertyNameNoUnderscores + "_" + ShaderUtils::generateName();
                sourceType = glslType(value);
                if(!metaProperty.hasNotifySignal()) {
                    qWarning() << "ShaderNode: property" << propertyName << "has no notification signal in" << this << "object with name" << name();
                }
                qDebug() << "Making uniform" << targetIdentifier;
                shaderBuilder->addUniform(this, propertyName, targetIdentifier, value, metaProperty);
            }
            // replaces '$property' or '$(property, type)'
            QRegularExpression namedRegex("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[a-z0-9]+\\s*\\))?");
            sourceContent.replace(namedRegex, ShaderUtils::convert(sourceType, targetType, targetIdentifier));
            alreadyReplaced.append(propertyName);
        }
    }

    setShaderBuilder(shaderBuilder);
    m_hasSetup = true;
    m_resolvedSource = sourceContent;
    return success;
}

QString ShaderNode::generateBody() const
{
    if(m_hasGeneratedBody) {
        return QString();
    }
    if(m_type.isEmpty()) {
        qWarning() << "WARNING: ShaderNode" << m_name << "is missing type.";
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
    // GLSL doesn't allow double underscores in names. Remove all before generating an identifier name.
    m_identifier = QString(name).replace("_", "") + "_" + ShaderUtils::generateName();

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

void ShaderNode::setHeaderFile(QUrl headerFile)
{
    if (m_headerFile == headerFile)
        return;

    QString fileName = QQmlFile::urlToLocalFileOrQrc(headerFile);
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    setHeader(file.readAll());

    m_headerFile = headerFile;
    emit headerFileChanged(headerFile);
}

ShaderBuilder *ShaderNode::shaderBuilder() const
{
    return m_shaderBuilder;
}

void ShaderNode::setShaderBuilder(ShaderBuilder *shaderBuilder)
{
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

QQmlListProperty<ShaderNode> ShaderNode::dependencies()
{
    return QQmlListProperty<ShaderNode>(this, m_declaredDependencies);
}

QUrl ShaderNode::headerFile() const
{
    return m_headerFile;
}
