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
        qWarning() << "ShaderUtils::glslType(): could not identify type" << value.typeName();
        return QString("float");
        break;
    }
}

QString ShaderUtils::convert(const QString &sourceType, const QString &targetType, const QString &identifier)
{
    const QString &v = identifier;
    if(targetType.isEmpty() || sourceType == targetType) {
        return v;
    }

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
    if(conversions.contains(sourceType)) {
        QVariantMap typeConversions = conversions[sourceType].toMap();
        if(typeConversions.contains(targetType)) {
            return "(" + typeConversions[targetType].toString() + ")";
        }
    }
    qWarning() << "ShaderUtils::convert(): No known conversion from " << sourceType << " to " << targetType;
    return v;
}
