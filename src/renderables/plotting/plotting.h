#pragma once
#ifndef PLOTTING_H
#define PLOTTING_H

#include "../../core/renderable.h"

#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QColor>
class Simulator;

class Plotting;
class PlottingRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    PlottingRenderer();
private:
    virtual void synchronize(Renderable *renderable) override;
    virtual void render() override;

    void uploadVBO(Plotting* plotting);
    void beforeLinkProgram() override;
    int m_vertexCount = 0;
    bool m_isInitialized = false;
    QVector4D m_color = QVector4D(0.0, 1.0, 0.0, 1.0);
};

class Plotting : public Renderable
{
    Q_OBJECT
public:
    Plotting();
    ~Plotting();
    virtual RenderableRenderer* createRenderer();

    void setPositions(QVector<QVector3D> &positions);
    QVector<QVector3D> &positions();

private:
    QVector<QVector3D> m_vertices;

private:
    friend class PlottingRenderer;
};

#endif // PLOTTING_H
