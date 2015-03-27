#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

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
    };
}

class Renderable; class Camera;
class RenderableRenderer : public QObject
{
    Q_OBJECT
protected:
    void generateVBOs();
    unsigned int m_numberOfVBOs = 0;
    bool m_shadersDirty = true;
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
    void setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString shaderCode);
    void setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName);
signals:

private:
    void prepareAndRender();
    void removeShader(QOpenGLShader::ShaderType type);
    void addShaderCodeToBase(QOpenGLShader::ShaderType type, QString shaderCode);

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
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
public:
    explicit Renderable(QObject *parent = 0);
    ~Renderable();

    virtual RenderableRenderer* createRenderer() = 0;

    void requestRender();
    void requestSynchronize();
    bool visible() const;
    Camera* camera() const;

signals:

    void visibleChanged(bool arg);
    void cameraChanged(Camera* arg);

public slots:

    void setVisible(bool arg);
    void setCamera(Camera* arg);

protected:

private:
    RenderableRenderer* m_renderer;
    bool m_visible = true;
    Camera* m_camera = 0;
};

#endif // RENDERABLE_H
