#include "variantshadernode.h"

VariantShaderNode::VariantShaderNode(QObject *parent)
    : ShaderNode(parent)
{

}

QVariant VariantShaderNode::value() const
{
    return m_value;
}

QString VariantShaderNode::generateHeader() const
{
    if(m_hasGeneratedHeader) {
        return QString();
    }
    QString headerResult = ShaderNode::generateHeader();
    headerResult += "uniform " + type() + " " + identifier() + ";\n";
    return headerResult;
}

QString VariantShaderNode::generateBody() const
{
    return QString();
}

void VariantShaderNode::setValue(QVariant value)
{
    if (m_value == value)
        return;

    m_value = value;
    ShaderNode *node = qvariant_cast<ShaderNode*>(value);
    if(!node) {
        setType(glslTypeFromVariant(value));
    }
    emit valueChanged(value);
}
