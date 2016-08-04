#include "customcolor.h"

CustomColor::CustomColor()
{

}


QString CustomColor::fragmentShaderDefines()
{
    return QString("\n #define CUSTOMCOLOR\n ");
}

QString CustomColor::vertexShaderDefines()
{
    return QString("");
}

QString CustomColor::fragmentShaderLibrary()
{
    QString shaderLibrary = QString(
"highp vec3 applyCustomColor(highp vec3 position, highp vec3 normal, highp vec3 light) {\n "
"highp vec3 customColor = light;\n\n ");
shaderLibrary.append(m_code);
shaderLibrary.append(QString(
"\n "
"return customColor;\n "
"}"));
    return shaderLibrary;
}

QString CustomColor::vertexShaderLibrary()
{
    return QString("");
}

CustomColor *CustomColor::clone()
{
    CustomColor *clone = new CustomColor();
    clone->copyState(this);

    return clone;
}

void CustomColor::copyState(ShaderEffect *source)
{
    CustomColor *customColor = qobject_cast<CustomColor*>(source);
    if(customColor) {
        setCode(customColor->code());
    }
}

void CustomColor::beforeRendering(QOpenGLShaderProgram &shaderProgram)
{
    // shaderProgram.setUniformValue(shaderProgram.uniformLocation(QString("cp_lights[%1].ambientColor").arg(m_lightId)), m_ambientColor);
}

void CustomColor::afterRendering(QOpenGLShaderProgram &shaderProgram)
{

}

QString CustomColor::code() const
{
    return m_code;
}

void CustomColor::setCode(QString code)
{
    if (m_code == code)
        return;

    m_code = code;
    emit codeChanged(code);
    setShadersDirty(true);
}
