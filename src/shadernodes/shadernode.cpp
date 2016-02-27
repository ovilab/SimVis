#include "shadernode.h"

#include <QDebug>

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

bool ShaderNode::isUniform() const
{
    return m_isUniform;
}

QList<QVariant> ShaderNode::depends() const
{
    return m_depends;
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
    m_hasGeneratedHeader = true;
    return QString();
}

QString ShaderNode::convert(QString targetType) const
{
    if(m_type == targetType) {
        return m_identifier;
    }
    QString v = m_identifier;

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
    return m_identifier;
}

QList<QVariant> ShaderNode::findUniforms() const
{
    QList<QVariant> uniforms;
    for(ShaderNode* node : m_resolvedDependencies) {
        uniforms.append(node->findUniforms());
    }
    if(isUniform()) {
//        uniforms.append(QVariant(this));
    }
    return uniforms;
}

QString ShaderNode::generateBody() const
{
    if(m_hasGeneratedBody) {
        return QString();
    }
    if(m_isUniform) {
        return QString();
    }
    if(m_type.isEmpty()) {
        qWarning() << "WARNING: ShaderNode" << m_name << m_identifier << "is missing type.";
        return QString();
    }
    QString body = "";
    for(ShaderNode* dependency : m_resolvedDependencies) {
        body += dependency->generateBody();
    }
    body += m_type + " " + m_identifier + ";\n";
    if(!m_initialization.isEmpty()) {
        body += m_initialization + ";\n";
    }
    if(!m_result.isEmpty()) {
        body += m_identifier + " = " + m_result + ";\n";
    }
    body += "\n";
    m_hasGeneratedBody = true;
    return body;
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

void ShaderNode::setIsUniform(bool isUniform)
{
    if (m_isUniform == isUniform)
        return;

    m_isUniform = isUniform;
    emit isUniformChanged(isUniform);
}

void ShaderNode::setDepends(QList<QVariant> depends)
{
    if (m_depends == depends)
        return;

    m_depends = depends;
    resolveDependencies();
    emit dependsChanged(depends);
}

void ShaderNode::resolveDependencies()
{
    m_resolvedDependencies.clear();
    for(const QVariant &variant : m_depends) {
        ShaderNode* otherNode = qvariant_cast<ShaderNode*>(variant);
        if(otherNode) {
            m_resolvedDependencies.append(otherNode);
        } else {
            // TODO convert regular variants to ShaderNode automatically
            qDebug() << "Got unresolvable variant dependency";
        }
    }
}
