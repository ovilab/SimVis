#include "shadernode.h"
#include "shaderbuilder.h"
#include "shaderutils.h"
#include "shadergroup.h"
#include "shaderbuilderbinding.h"

#include <QDebug>
#include <QJSValueIterator>
#include <QMetaProperty>
#include <QRegularExpression>
#include <QSignalMapper>
#include <QQmlFile>
#include <QFile>

ShaderNode::ShaderNode(Qt3DCore::QNode *parent)
    : Qt3DCore::QNode(parent)
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
    for(const ShaderNode *node : m_resolvedDependencies) {
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

bool ShaderNode::setup(ShaderBuilder* shaderBuilder, QString tempIdentifier)
{
    if(m_hasSetup && tempIdentifier.isEmpty()) {
        return true;
    }
    if(!m_requirement) {
        qWarning() << "ERROR: ShaderNode::setup(): Requirement for" << this << name() << "is not satisfied.";
        return false;
    }
    if(m_type.isEmpty()) {
        qWarning() << "ERROR: ShaderNode::setup(): " << name() << "is missing type.";
        return false;
    }
    QString currentIdentifier;
    if(tempIdentifier.isEmpty()) {
        currentIdentifier = identifier();
    } else {
        currentIdentifier = tempIdentifier;
    }

    QString sourceContent;

    sourceContent += m_source;
    if(!m_result.isEmpty()) {
        sourceContent += currentIdentifier + " = " + m_result + ";\n";
    }

    m_resolvedDependencies.clear();

    QVariantMap mappings = m_mappings;
    for(int i = 0; i < metaObject()->propertyCount(); i++) {
        QString propertyName = metaObject()->property(i).name();
        if(mappings.contains(propertyName)) {
            continue;
        }
        mappings.insert(propertyName, metaObject()->property(i).read(this));
    }

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
            if(!mappings.contains(propertyName)) {
                // No connected property, assume internal variable that just needs a unique name
                QString propertylessIdentifier = propertyNameNoUnderscores + "_" + ShaderUtils::generateName();
                QRegularExpression namedRegex("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*[_a-zA-Z0-9]+\\s*\\))?");
                sourceContent.replace(namedRegex, propertylessIdentifier);
                alreadyReplaced.append(propertyName);
                continue;
            }
            QVariant value = mappings[propertyName];

            ShaderGroup *shaderGroup = qvariant_cast<ShaderGroup*>(value);
            if(shaderGroup) {
                int i = 0;
                for(ShaderNode *groupNode : shaderGroup->m_nodes) {
                    success = success && groupNode->setup(shaderBuilder);
                    if(!m_resolvedDependencies.contains(groupNode)) {
                        m_resolvedDependencies.append(groupNode);
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
                        if(!m_resolvedDependencies.contains(declaredDependency)) {
                            m_resolvedDependencies.append(declaredDependency);
                        }
                    }
                }
                success = success && node->setup(shaderBuilder);
                if(!m_resolvedDependencies.contains(node)) {
                    m_resolvedDependencies.append(node);
                }
                targetIdentifier = node->identifier();
                sourceType = node->type();
            } else {
                //
                // make a uniform
                QByteArray propertyNameArray = propertyName.toUtf8();
                int propertyIndex = metaObject()->indexOfProperty(propertyNameArray.constData());
//                if(propertyIndex < 0) {
//                    qWarning() << "WARNING: ShaderNode::setup(): Mapping with name" << propertyName << "on" << name()
//                               << "is neither a shader node nor a property. This is not supported.";
//                    continue;
//                }
                QMetaProperty metaProperty = metaObject()->property(propertyIndex);

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
                shaderBuilder->addUniform(this, propertyName, targetIdentifier, value, metaProperty);
            }
            // replaces '$property' or '$(property, type)'
            QRegularExpression namedRegex("\\$(\\(\\s*)?" + propertyName + "(\\s*,\\s*" + targetType + "\\s*\\))?");
            sourceContent.replace(namedRegex, ShaderUtils::convert(sourceType, targetType, targetIdentifier));
            alreadyReplaced.append(propertyName);
        }
    }

    setShaderBuilder(shaderBuilder);
    if(!tempIdentifier.isEmpty()) {
        m_hasSetup = true;
    }
    m_resolvedSource = sourceContent;
    return success;
}

QString ShaderNode::generateBody() const
{
    if(m_hasGeneratedBody) {
        return QString();
    }
    if(m_type.isEmpty()) {
        qWarning() << "WARNING: ShaderNode::generateBody(): " << m_name << "is missing type.";
        return QString();
    }
    QString body = "";
    for(ShaderNode* dependency : m_resolvedDependencies) {
        body += dependency->generateBody();
    }
    body += m_type + " " + identifier() + ";\n";
    if(!m_resolvedSource.isEmpty()) {
        body += m_resolvedSource + "\n";
    }
    body += "\n";
    m_hasGeneratedBody = true;
    return body;
}

void ShaderNode::reset() const
{
    for(ShaderNode* dependency : m_resolvedDependencies) {
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

void ShaderNode::setSourceWithoutNotification(QString source)
{
    m_source = source;
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

void ShaderNode::addMapping(QString propertyName, const QVariant &value)
{
    m_mappings.insert(propertyName, value);
}

void ShaderNode::removeMapping(QString propertyName)
{
    m_mappings.remove(propertyName);
}

void ShaderNode::addDependency(ShaderNode *dependency)
{
    m_declaredDependencies.append(dependency);
}

void ShaderNode::removeDependency(ShaderNode *dependency)
{
    m_declaredDependencies.removeAll(dependency);
}

void ShaderNode::clearDependencies()
{
    m_declaredDependencies.clear();
}

QList<ShaderNode *> ShaderNode::resolvedDependencies() const
{
    return m_resolvedDependencies;
}

QString ShaderNode::resolvedSource() const
{
    return m_resolvedSource;
}
