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
    virtual void synchronize(Renderable* renderable) = 0;
    virtual void render() = 0;
    virtual void createShaderProgram() = 0;
    void generateVBOs();
    unsigned int m_numberOfVBOs = 0;
    std::vector<GLuint> m_vboIds;

    QOpenGLShaderProgram* m_program = 0;
    QOpenGLFunctions* glFunctions();
    QOpenGLFunctions* m_funcs = 0;

private:

    friend class Renderable;
};

class Renderable : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
public:
    explicit Renderable(QObject *parent = 0);
    ~Renderable();

    virtual void afterSynchronize();
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

    void setVisible(bool arg)
    {
        if (m_visible == arg)
            return;

        m_visible = arg;
        emit visibleChanged(arg);
    }

protected:

private:
    RenderableRenderer* m_renderer;
    bool m_visible = true;
};

#endif // RENDERABLE_H
