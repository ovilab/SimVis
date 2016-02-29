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

struct VboEnableHelperData {
    int location;
    int size;
};

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

    template<typename U>
    friend class VertexAttributeArrayHelper;
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

template<typename U>
class VertexAttributeArrayHelper
{
public:
    VertexAttributeArrayHelper(RenderableRenderer *renderer)
        : m_renderer(renderer)
    {

    }

    ~VertexAttributeArrayHelper() {
        for(int i : m_registeredLocations) {
            m_renderer->program().disableAttributeArray(i);
        }
        m_registeredLocations.clear();
    }

    void enableVboAttribute(GLint count, GLenum type, GLuint location, quintptr offset){
        m_registeredLocations.append(location);
        m_renderer->program().enableAttributeArray(location);
        m_renderer->glFunctions()->glVertexAttribPointer(location, count, type, GL_FALSE, m_stride, (const void *)offset);
    }

    void enableVboAttribute(const QVector3D&, GLuint location, quintptr offset) {
        enableVboAttribute(3, GL_FLOAT, location, offset);
    }

    void enableVboAttribute(const QVector2D&, GLuint location, quintptr offset) {
        enableVboAttribute(2, GL_FLOAT, location, offset);
    }

    void enableVboAttribute(const float&, GLuint location, quintptr offset) {
        enableVboAttribute(1, GL_FLOAT, location, offset);
    }

    void enableVboAttribute(const int&, GLuint location, quintptr offset) {
        enableVboAttribute(1, GL_INT, location, offset);
    }

    template<typename T>
    void addData(const T &member, GLuint location) {
        enableVboAttribute(member, location, m_offset);
        m_location = location;
        m_offset += sizeof(T);
    }

    template<typename T>
    void addData(const T &member, QString location) {
        int resolvedLocation = m_renderer->program().attributeLocation(location);
        // TODO consider not adding if location = -1. Requires ensuring offset is still correct.
        addData(member, resolvedLocation);
    }

    template<typename T>
    void addData(const T &member) {
        m_location += 1;
        addData(member, m_location);
    }

    U* operator()() { return nullptr; }

private:
    RenderableRenderer *m_renderer = nullptr;
    QVector<int> m_registeredLocations;
    GLuint m_location = 0;
    quintptr m_offset = 0;
    GLsizei m_stride = sizeof(U);
};


#endif // RENDERABLE_H
