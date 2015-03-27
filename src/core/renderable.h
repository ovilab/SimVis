#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QColor>
#include <QElapsedTimer>

namespace CompPhys {
    enum Shader
    {
        AllShaders,
        Perlin2,
        Perlin3,
        Perlin4,
        Simplex2,
        Simplex3,
        Simplex4,
        ColorEffects,
        Light,
    };
}

class Renderable; class Camera;
class RenderableRenderer : public QObject
{
    Q_OBJECT
public:
    RenderableRenderer();
protected:
    void generateVBOs();
    unsigned int m_numberOfVBOs = 0;
    bool m_shadersDirty = true;
    QElapsedTimer m_elapsedTime;
    QColor m_ambient;
    QColor m_diffuse;
    QColor m_specular;
    QVector3D m_lightPosition;
    float m_diffuseIntensity;
    float m_ambientIntensity;
    float m_shininess;
    float m_attenuation;
    QMatrix4x4 m_modelViewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QVector3D m_viewVector;
    QVector3D m_cameraPosition;
    QVector<GLuint> m_vboIds;
    QString m_fragmentShaderBase;
    QString m_vertexShaderBase;
    QOpenGLShaderProgram& program();
    QOpenGLFunctions* glFunctions();
    QString contentFromFile(QString fileName);
    void addShaderLibrary(QOpenGLShader::ShaderType type, CompPhys::Shader shader);
    void addShaderCodeToBase(QOpenGLShader::ShaderType type, QString shaderCode);
    void setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString shaderCode);
    void setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName);
signals:

private:
    void prepareAndRender();
    void removeShader(QOpenGLShader::ShaderType type);

    virtual void beforeLinkProgram() = 0;
    virtual void synchronize(Renderable* renderable) = 0;
    virtual void render() = 0;

    QOpenGLShaderProgram m_program;
    QOpenGLFunctions* m_funcs = 0;

    friend class Renderable;
};

class Renderable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(QColor ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
    Q_PROPERTY(QColor diffuse READ diffuse WRITE setDiffuse NOTIFY diffuseChanged)
    Q_PROPERTY(QColor specular READ specular WRITE setSpecular NOTIFY specularChanged)
    Q_PROPERTY(float diffuseIntensity READ diffuseIntensity WRITE setDiffuseIntensity NOTIFY diffuseIntensityChanged)
    Q_PROPERTY(float ambientIntensity READ ambientIntensity WRITE setAmbientIntensity NOTIFY ambientIntensityChanged)
    Q_PROPERTY(float shininess READ shininess WRITE setShininess NOTIFY shininessChanged)
    Q_PROPERTY(float attenuation READ attenuation WRITE setAttenuation NOTIFY attenuationChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QVector3D lightPosition READ lightPosition WRITE setLightPosition NOTIFY lightPositionChanged)
public:
    explicit Renderable(QObject *parent = 0);
    ~Renderable();

    virtual RenderableRenderer* createRenderer() = 0;

    void requestRender();
    void requestSynchronize();
    bool visible() const;
    Camera* camera() const;
    QColor ambient() const;
    QColor diffuse() const;
    QVector3D lightPosition() const;
    float diffuseIntensity() const;
    float ambientIntensity() const;
    float shininess() const;
    QColor specular() const;
    float attenuation() const;

signals:

    void visibleChanged(bool arg);
    void cameraChanged(Camera* arg);
    void ambientChanged(QColor arg);
    void diffuseChanged(QColor arg);
    void lightPositionChanged(QVector3D arg);
    void diffuseIntensityChanged(float arg);
    void ambientIntensityChanged(float arg);
    void shininessChanged(float arg);
    void specularChanged(QColor arg);
    void attenuationChanged(float arg);

public slots:

    void setVisible(bool arg);
    void setCamera(Camera* arg);
    void setAmbient(QColor arg);
    void setDiffuse(QColor arg);
    void setLightPosition(QVector3D arg);
    void setDiffuseIntensity(float arg);
    void setAmbientIntensity(float arg);
    void setShininess(float arg);
    void setSpecular(QColor arg);
    void setAttenuation(float arg);

private:
    RenderableRenderer* m_renderer;
    bool m_visible = true;
    Camera* m_camera = 0;
    QColor m_ambient;
    QColor m_diffuse;
    QColor m_specular;
    QVector3D m_lightPosition;
    float m_diffuseIntensity = 1.0;
    float m_ambientIntensity = 0.1;
    float m_shininess = 20.0;
    float m_attenuation = 0.01;
};

#endif // RENDERABLE_H
