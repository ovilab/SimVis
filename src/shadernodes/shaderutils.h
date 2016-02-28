#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <QString>
#include <QVariant>

class ShaderUtils
{
public:
    static QString glslType(const QVariant &value);
    static QString convert(const QString &sourceType, const QString &targetType, const QString &identifier);
};

#endif // SHADERUTILS_H
