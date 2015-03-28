#include "defaultlight.h"

QString DefaultLight::fragmentShaderDefines()
{
    QString defines = "\n #define DEFAULTLIGHT \n";
    return defines;
}

QString DefaultLight::vertexShaderDefines()
{
    QString defines = "\n #define DEFAULTLIGHT \n";
    return defines;
}

QString DefaultLight::fragmentShaderLibrary()
{
    QString shaderLibrary;
    shaderLibrary.append(contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/light.glsl"));
    return shaderLibrary;
}

QString DefaultLight::vertexShaderLibrary()
{
    QString shaderLibrary;
    shaderLibrary.append(contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/light.glsl"));
    return shaderLibrary;
}

void DefaultLight::copyState(ShaderEffect *source) {
    DefaultLight *defaultLight = qobject_cast<DefaultLight*>(source);
    m_ambientColor = defaultLight->ambientColor();
    m_diffuseColor = defaultLight->diffuseColor();
    m_specularColor = defaultLight->specularColor();
    m_shininess = defaultLight->shininess();
    m_attenuation = defaultLight->attenuation();
    m_diffuseIntensity = defaultLight->diffuseIntensity();
    m_specularIntensity = defaultLight->specularIntensity();
    m_ambientIntensity = defaultLight->ambientIntensity();
    m_position = defaultLight->position();
    m_enabled = source->enabled();
    m_shadersDirty = source->shadersDirty();
}

DefaultLight *DefaultLight::clone()
{
    DefaultLight *clone = new DefaultLight();
    clone->copyState(this);

    return clone;
}

void DefaultLight::setUniformValues(QOpenGLShaderProgram &shaderProgram)
{
    shaderProgram.setUniformValue("cp_ambientColor", m_ambientColor);
    shaderProgram.setUniformValue("cp_diffuseColor", m_diffuseColor);
    shaderProgram.setUniformValue("cp_specularColor", m_specularColor);
    shaderProgram.setUniformValue("cp_shininess", m_shininess);
    shaderProgram.setUniformValue("cp_attenuation", m_attenuation);
    shaderProgram.setUniformValue("cp_diffuseIntensity", m_diffuseIntensity);
    shaderProgram.setUniformValue("cp_specularIntensity", m_specularIntensity);
    shaderProgram.setUniformValue("cp_ambientIntensity", m_ambientIntensity);
    shaderProgram.setUniformValue("cp_lightPosition", m_position);
}

QColor DefaultLight::ambientColor() const
{
    return m_ambientColor;
}

QColor DefaultLight::diffuseColor() const
{
    return m_diffuseColor;
}

QColor DefaultLight::specularColor() const
{
    return m_specularColor;
}

float DefaultLight::diffuseIntensity() const
{
    return m_diffuseIntensity;
}

float DefaultLight::ambientIntensity() const
{
    return m_ambientIntensity;
}

float DefaultLight::specularIntensity() const
{
    return m_specularIntensity;
}

float DefaultLight::shininess() const
{
    return m_shininess;
}

float DefaultLight::attenuation() const
{
    return m_attenuation;
}

QVector3D DefaultLight::position() const
{
    return m_position;
}

void DefaultLight::setAmbientColor(QColor arg)
{
    if (m_ambientColor == arg)
        return;

    m_ambientColor = arg;
    emit ambientColorChanged(arg);
}

void DefaultLight::setDiffuseColor(QColor arg)
{
    if (m_diffuseColor == arg)
        return;

    m_diffuseColor = arg;
    emit diffuseColorChanged(arg);
}

void DefaultLight::setSpecularColor(QColor arg)
{
    if (m_specularColor == arg)
        return;

    m_specularColor = arg;
    emit specularColorChanged(arg);
}

void DefaultLight::setDiffuseIntensity(float arg)
{
    if (m_diffuseIntensity == arg)
        return;

    m_diffuseIntensity = arg;
    emit diffuseIntensityChanged(arg);
}

void DefaultLight::setAmbientIntensity(float arg)
{
    if (m_ambientIntensity == arg)
        return;

    m_ambientIntensity = arg;
    emit ambientIntensityChanged(arg);
}

void DefaultLight::setSpecularIntensity(float arg)
{
    if (m_specularIntensity == arg)
        return;

    m_specularIntensity = arg;
    emit specularIntensityChanged(arg);
}

void DefaultLight::setShininess(float arg)
{
    if (m_shininess == arg)
        return;

    m_shininess = arg;
    emit shininessChanged(arg);
}

void DefaultLight::setAttenuation(float arg)
{
    if (m_attenuation == arg)
        return;

    m_attenuation = arg;
    emit attenuationChanged(arg);
}

void DefaultLight::setPosition(QVector3D arg)
{
    if (m_position == arg)
        return;

    m_position = arg;
    emit positionChanged(arg);
}

