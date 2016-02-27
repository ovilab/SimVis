#include "shaderoutput.h"

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

ShaderNode *ShaderOutput::value() const
{
    return m_value;
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

void ShaderOutput::setValue(ShaderNode *value)
{
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(value);
}
