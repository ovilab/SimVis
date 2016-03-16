#include "shaderbuilderbinding.h"
#include "shaderbuilder.h"

/*!
 * \class ShaderBuilderBinding
 *
 * \brief Can be used as a special purpose shader node that binds to properties
 * in the instanced ShaderBuilder.
 */

ShaderBuilderBinding::ShaderBuilderBinding(QObject *parent)
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

bool ShaderBuilderBinding::setup(ShaderBuilder *shaderBuilder)
{
    m_dependencies.clear();
    QByteArray bindingName = m_property.toUtf8();
    ShaderNode *node = qvariant_cast<ShaderNode*>(shaderBuilder->property(bindingName.constData()));
    if(!node) {
        qWarning() << "ERROR: ShaderBuilderBinding bound to a non-ShaderNode property named" << m_property;
        return false;
    }
    m_dependencies.append(node);
    node->setup(shaderBuilder);
    setType(node->type());
    m_resolvedSource = identifier() + " = " + node->identifier() + ";\n";
    return true;
}
