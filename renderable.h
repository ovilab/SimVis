#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class Renderable; class Camera;
class RenderableRenderer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
protected:
    void generateVBOs();
    unsigned int m_numberOfVBOs = 0;
    QVector<GLuint> m_vboIds;
    QOpenGLShaderProgram& program();
    QOpenGLFunctions* glFunctions();
public:
    Camera* camera() const
    {
        return m_camera;
    }

public slots:
    void setCamera(Camera* arg)
    {
        if (m_camera == arg)
            return;

        m_camera = arg;
        emit cameraChanged(arg);
    }

signals:
    void cameraChanged(Camera* arg);

private:
    void prepareAndRender();
    virtual void beforeLinkProgram() = 0;
    virtual void synchronize(Renderable* renderable) = 0;
    virtual void render() = 0;

    QOpenGLShaderProgram m_program;
    QOpenGLFunctions* m_funcs = 0;

    friend class Renderable;
    Camera* m_camera;
};






class Renderable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit Renderable(QObject *parent = 0);
    ~Renderable();

    virtual RenderableRenderer* createRenderer() = 0;

    void requestRender();
    void requestSynchronize();

    bool visible() const
    {
        return m_visible;
    }

signals:

    void visibleChanged(bool arg);

public slots:

    void setVisible(bool arg);

protected:

private:
    RenderableRenderer* m_renderer;
    bool m_visible = true;
};

#endif // RENDERABLE_H
