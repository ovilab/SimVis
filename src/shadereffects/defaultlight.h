#ifndef DEFAULTLIGHT_H
#define DEFAULTLIGHT_H

#include "../core/shadereffect.h"

#include <QColor>
#include <QVector3D>

class DefaultLight : public ShaderEffect
{
    Q_OBJECT
    Q_PROPERTY(QColor ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(QColor specular READ specular WRITE setSpecular NOTIFY specularChanged)
    Q_PROPERTY(float diffuseIntensity READ diffuseIntensity WRITE setDiffuseIntensity NOTIFY diffuseIntensityChanged)
    Q_PROPERTY(float ambientIntensity READ ambientIntensity WRITE setAmbientIntensity NOTIFY ambientIntensityChanged)
    Q_PROPERTY(float specularIntensity READ specularIntensity WRITE setSpecularIntensity NOTIFY specularIntensityChanged)
    Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
    Q_PROPERTY(float attenuation READ attenuation WRITE setAttenuation NOTIFY attenuationChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)

public:
    // ShaderEffect interface
    QString fragmentShaderDefines() override;
    QString vertexShaderDefines() override;
    QString fragmentShaderLibrary() override;
    QString vertexShaderLibrary() override;
    DefaultLight *clone() override;
    void setUniformValues(QOpenGLShaderProgram &shaderProgram) override;
    void copyState(ShaderEffect *source) override;
    QColor ambient() const;
    QColor diffuse() const;
    QColor specular() const;
    float diffuseIntensity() const;
    float ambientIntensity() const;
    float specularIntensity() const;
    float shininess() const;
    float attenuation() const;
    QVector3D position() const;

public slots:
    void setAmbient(QColor arg);
    void setDiffuse(QColor arg);
    void setSpecular(QColor arg);
    void setDiffuseIntensity(float arg);
    void setAmbientIntensity(float arg);
    void setSpecularIntensity(float arg);
    void setShininess(float arg);
    void setAttenuation(float arg);
    void setPosition(QVector3D arg);

signals:
    void ambientChanged(QColor arg);
    void diffuseChanged(QColor arg);
    void specularChanged(QColor arg);
    void diffuseIntensityChanged(float arg);
    void ambientIntensityChanged(float arg);
    void specularIntensityChanged(float arg);
    void shininessChanged(float arg);
    void attenuationChanged(float arg);
    void positionChanged(QVector3D arg);

private:
    QColor m_ambient;
    QColor m_diffuse;
    QColor m_specular;
    float m_diffuseIntensity = 1.0;
    float m_ambientIntensity = 0.1;
    float m_specularIntensity = 0.1;
    float m_shininess = 30.0;
    float m_attenuation = 0.01;
    QVector3D m_position;
};

#endif // DEFAULTLIGHT_H
