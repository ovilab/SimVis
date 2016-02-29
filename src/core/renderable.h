#ifndef RENDERABLE_H
#define RENDERABLE_H
#include "shadereffect.h"

#include <cstdarg>

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>
#include <QColor>
#include <QElapsedTimer>
#include <QQuickItem>

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
        Tools
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
    unsigned int m_vboCount = 0;
    bool m_shadersDirty = true;
    QList<ShaderEffect*> m_shaderEffects;
    QElapsedTimer m_elapsedTime;
    QMatrix4x4 m_modelViewMatrix;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrixInverse;
    QMatrix4x4 m_projectionMatrixInverse;
    QVector3D m_viewVector;
    QVector3D m_viewCenter;
    QVector3D m_upVector;
    QVector3D m_rightVector;
    QVector3D m_cameraPosition;
    QVector<GLuint> m_vboIds;
    QOpenGLVertexArrayObject *m_vao;
    QString m_fragmentShaderBase;
    QString m_geometryShaderBase;
    QString m_vertexShaderBase;
    QOpenGLShaderProgram& program();
    QOpenGLFunctions* glFunctions();
    QString contentFromFile(QString fileName);
    void addShaderLibrary(QOpenGLShader::ShaderType type, CompPhys::Shader shader);
    void addShaderCodeToBase(QOpenGLShader::ShaderType type, QString shaderCode);
    void setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString shaderCode);
    void setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName);
    bool geometryShaderIsSupported();
    QString generateGLSLHeader();

    void enableVboAttribute(int location, GLint count, GLenum type, GLsizei stride, quintptr offset);

    void enableVboObjectMemberHelper(int location, GLsizei stride, quintptr offset, const QVector3D&);
    void enableVboObjectMemberHelper(int location, GLsizei stride, quintptr offset, float);

    template<typename T>
    void enableVboObjectHelper(int location, GLsizei stride, quintptr offset, T member);

    template<typename T, typename... Args>
    void enableVboObjectHelper(int location, GLsizei stride, quintptr offset, T member, Args... args);

    template<typename T, typename... Args>
    void enableVboObject(T object, Args... args);

    template<typename T>
    void disableVboObjectHelper(int location, T member);

    template<typename T, typename... Args>
    void disableVboObjectHelper(int location, T member, Args... args);

    template<typename T, typename... Args>
    void disableVboObject(T object, Args... args);
signals:

private:
    void prepareAndRender();
    void removeShader(QOpenGLShader::ShaderType type);
    void copyShaderEffects(Renderable *renderable);

    virtual void beforeLinkProgram() = 0;
    virtual void synchronize(Renderable* renderable) = 0;
    virtual void render() = 0;

    QOpenGLShaderProgram m_program;
    QOpenGLFunctions* m_funcs = 0;

    friend class Renderable;
};

class Renderable : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    explicit Renderable(QQuickItem *parent = 0);
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
private:
    RenderableRenderer* m_renderer;
    bool m_visible = true;
    Camera* m_camera = 0;
};

template<typename T>
inline void RenderableRenderer::enableVboObjectHelper(int location, GLsizei stride, quintptr offset, T member) {
    enableVboObjectMemberHelper(location, stride, offset, member);
}

template<typename T, typename... Args>
inline void RenderableRenderer::enableVboObjectHelper(int location, GLsizei stride, quintptr offset, T member, Args... args)
{
    enableVboObjectHelper(location, stride, offset, member);
    enableVboObjectHelper(location+1, stride, offset+sizeof(member), args...);
}

template<typename T, typename... Args>
inline void RenderableRenderer::enableVboObject(T object, Args... args)
{
    enableVboObjectHelper(0, sizeof(object), 0, args...);
}

template<typename T>
inline void RenderableRenderer::disableVboObjectHelper(int location, T member) {
    program().disableAttributeArray(location);
}

template<typename T, typename... Args>
inline void RenderableRenderer::disableVboObjectHelper(int location, T member, Args... args)
{
    disableVboObjectHelper(location, member);
    disableVboObjectHelper(location+1, args...);
}

template<typename T, typename... Args>
inline void RenderableRenderer::disableVboObject(T object, Args... args)
{
    disableVboObjectHelper(0, args...);
}

#endif // RENDERABLE_H
