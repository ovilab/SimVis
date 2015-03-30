#include "shadernode.h"

#include <QUuid>
#include <QRegularExpression>

ShaderNode::ShaderNode(QQuickItem *parent) : QQuickItem(parent)
{
    setObjectName("a" + QUuid::createUuid().toString().replace(QRegularExpression("[{}-]"), "").left(8) + "_");
}

ShaderNode::~ShaderNode()
{

}

QObject *ShaderNode::input() const
{
    return m_input;
}

QObject *ShaderNode::output() const
{
    return m_output;
}

QString ShaderNode::shaderSource() const
{
    return m_shaderSource;
}

QStringList ShaderNode::inputs() const
{
    return m_inputs;
}

QStringList ShaderNode::outputs() const
{
    return m_outputs;
}

void ShaderNode::setInput(QObject *arg)
{
    if (m_input == arg)
        return;

    m_input = arg;
    emit inputChanged(arg);
}

void ShaderNode::setOutput(QObject *arg)
{
    if (m_output == arg)
        return;

    m_output = arg;
    emit outputChanged(arg);
}

void ShaderNode::setShaderSource(QString arg)
{
    if (m_shaderSource == arg)
        return;

    m_shaderSource = arg;
    emit shaderSourceChanged(arg);
}

void ShaderNode::setInputs(QStringList arg)
{
    if (m_inputs == arg)
        return;

    m_inputs = arg;
    emit inputsChanged(arg);
}

void ShaderNode::setOutputs(QStringList arg)
{
    if (m_outputs == arg)
        return;

    m_outputs = arg;
    emit outputsChanged(arg);
}

