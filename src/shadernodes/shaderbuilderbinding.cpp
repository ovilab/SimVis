#include "shaderbuilderbinding.h"
#include "shaderbuilder.h"
#include "shaderutils.h"

/*!
 * \class ShaderBuilderBinding
 *
 * \brief Can be used as a special purpose shader node that binds to properties
 * in the instanced ShaderBuilder.
 */

ShaderBuilderBinding::ShaderBuilderBinding(Qt3DCore::QNode *parent)
    : ShaderNode(parent)
{
    setName("builderbinding");
}

QString ShaderBuilderBinding::property() const
{
    return m_property;
}

void ShaderBuilderBinding::setProperty(QString name)
{
    if (m_property == name)
        return;

    m_property = name;
    emit propertyChanged(name);
}

void ShaderBuilderBinding::setDefaultValue(QVariant defaultValue)
{
    if (m_defaultValue == defaultValue)
        return;

    m_defaultValue = defaultValue;
    emit defaultValueChanged(defaultValue);
}

bool ShaderBuilderBinding::setup(ShaderBuilder *shaderBuilder)
{
    m_dependencies.clear();
    QByteArray bindingName = m_property.toUtf8();
    ShaderNode *node = qvariant_cast<ShaderNode*>(shaderBuilder->property(bindingName.constData()));
    if(!node) {
        qWarning() << "WARNING: ShaderBuilderBinding::setup(): ShaderBuilderBinding bound"
                   << "to a non-ShaderNode property named" << m_property << "."
                   << "Using default value.";
        setType(ShaderUtils::glslType(defaultValue())); // TODO what if defaultValue type changes?
        setResult("$(defaultValue, " + type() + ")");
        return ShaderNode::setup(shaderBuilder);
    }
    m_dependencies.append(node);
    node->setup(shaderBuilder);
    setType(node->type());
    m_resolvedSource = identifier() + " = " + ShaderUtils::convert(node->type(), type(), node->identifier()) + ";\n";
    return true;
}

QVariant ShaderBuilderBinding::defaultValue() const
{
    return m_defaultValue;
}