#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <QString>
#include <QVariant>

class ShaderUtils
{
public:
    static QString glslType(const QVariant &value);
};

#endif // SHADERUTILS_H
