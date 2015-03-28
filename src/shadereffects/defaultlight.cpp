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
    shaderLibrary.append(contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/light.glsl"));
    return shaderLibrary;
}

QString DefaultLight::vertexShaderLibrary()
{
    QString shaderLibrary;
    shaderLibrary.append(contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/light.glsl"));
    return shaderLibrary;
}

void DefaultLight::copyState(ShaderEffect *source) {
    DefaultLight *defaultLight = qobject_cast<DefaultLight*>(source);
    setAmbient(defaultLight->ambient());
    setDiffuse(defaultLight->diffuse());
    setSpecular(defaultLight->specular());
    setShininess(defaultLight->shininess());
    setAttenuation(defaultLight->attenuation());
    setDiffuseIntensity(defaultLight->diffuseIntensity());
    setSpecularIntensity(defaultLight->specularIntensity());
    setAmbientIntensity(defaultLight->ambientIntensity());
    setPosition(defaultLight->position());
    setShadersDirty(defaultLight->shadersDirty());
}

DefaultLight *DefaultLight::clone()
{
    DefaultLight *clone = new DefaultLight();
    clone->copyState(this);

    return clone;
}

void DefaultLight::setUniformValues(QOpenGLShaderProgram &shaderProgram)
{
    shaderProgram.setUniformValue("cp_ambientColor", m_ambient);
    shaderProgram.setUniformValue("cp_diffuseColor", m_diffuse);
    shaderProgram.setUniformValue("cp_specularColor", m_specular);
    shaderProgram.setUniformValue("cp_shininess", m_shininess);
    shaderProgram.setUniformValue("cp_attenuation", m_attenuation);
    shaderProgram.setUniformValue("cp_diffuseIntensity", m_diffuseIntensity);
    shaderProgram.setUniformValue("cp_specularIntensity", m_specularIntensity);
    shaderProgram.setUniformValue("cp_ambientIntensity", m_ambientIntensity);
    shaderProgram.setUniformValue("cp_lightPosition", m_position);
}

QColor DefaultLight::ambient() const
{
    return m_ambient;
}

QColor DefaultLight::diffuse() const
{
    return m_diffuse;
}

QColor DefaultLight::specular() const
{
    return m_specular;
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

void DefaultLight::setAmbient(QColor arg)
{
    if (m_ambient == arg)
        return;

    m_ambient = arg;
    emit ambientChanged(arg);
}

void DefaultLight::setDiffuse(QColor arg)
{
    if (m_diffuse == arg)
        return;

    m_diffuse = arg;
    emit diffuseChanged(arg);
}

void DefaultLight::setSpecular(QColor arg)
{
    if (m_specular == arg)
        return;

    m_specular = arg;
    emit specularChanged(arg);
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

