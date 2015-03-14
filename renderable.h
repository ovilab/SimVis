#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <vector>

class Renderable;
class RenderableRenderer : public QObject
{
    Q_OBJECT
protected:
    void generateVBOs();
    unsigned int m_numberOfVBOs = 0;
    std::vector<GLuint> m_vboIds;
    QOpenGLShaderProgram& program();
    QOpenGLFunctions* glFunctions();

private:
    void prepareAndRender();
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
