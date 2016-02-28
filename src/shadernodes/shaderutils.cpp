#include "shaderutils.h"

#include "shadernode.h"

#include <QDebug>

QString ShaderUtils::glslType(const QVariant &value)
{
    ShaderNode *node = qvariant_cast<ShaderNode*>(value);
    if(node) {
        return node->type();
    }
    switch(value.type()) {
    case QVariant::Bool:
        return QString("bool");
        break;
    case QVariant::Int:
        return QString("int");
        break;
    case QVariant::Double:
        return QString("float");
        break;
    case QVariant::Vector2D:
        return QString("vec2");
        break;
    case QVariant::Vector3D:
        return QString("vec3");
        break;
    case QVariant::Vector4D:
        return QString("vec4");
        break;
    default:
        qWarning() << "WARNING: GlslVariantBridge could not identify type" << value.typeName();
        return QString("float");
        break;
    }
}
