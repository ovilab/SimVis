#include "shaderoutput.h"
#include "shaderutils.h"

#include <QVector3D>

ShaderOutput::ShaderOutput(QObject *parent) : QObject(parent)
{
}

QString ShaderOutput::name() const
{
    return m_name;
}

QString ShaderOutput::type() const
{
    return m_type;
}

QVariant ShaderOutput::value() const
{
    return m_value;
}

ShaderNode *ShaderOutput::node()
{
    return &m_node;
}

void ShaderOutput::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void ShaderOutput::setType(QString type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}

void ShaderOutput::setValue(const QVariant &value)
{
    if (m_value == value)
        return;

    m_node.setValue(value);

    m_value = value;
    emit valueChanged(value);
}

ShaderOutputPassthroughNode::ShaderOutputPassthroughNode(QObject *parent) :
    ShaderNode(parent)
{
    setName("passthrough");
    setResult("$value");
}

QVariant ShaderOutputPassthroughNode::value() const
{
    return m_value;
}

void ShaderOutputPassthroughNode::setValue(QVariant value)
{
    if (m_value == value)
        return;

    setType(ShaderUtils::glslType(value));

    m_value = value;
    emit valueChanged(value);
}
