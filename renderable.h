#pragma once
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <QObject>
#include <QOpenGLFunctions>

class Renderable;
class RenderableRenderer : public QObject
{
    Q_OBJECT
protected:
    virtual void synchronize(Renderable* renderable) = 0;
    virtual void render() = 0;
    unsigned int m_numberOfVBOs = 0;
    QList<GLuint> m_vboIds;

    QOpenGLFunctions* glFunctions();

private:
    QOpenGLFunctions* m_funcs = 0;

    friend class Renderable;
};

class Renderable : public QObject
{
    Q_OBJECT
public:
    explicit Renderable(QObject *parent = 0);
    ~Renderable();

    virtual void afterSynchronize();
    virtual RenderableRenderer* createRenderer() = 0;
    // virtual void createShaderProgram() = 0;

    void requestRender();
    void requestSynchronize();

signals:

public slots:

protected:

private:
    RenderableRenderer* m_renderer;
};

#endif // RENDERABLE_H
