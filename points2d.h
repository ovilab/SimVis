#pragma once
#ifndef POINTS2D_H
#define POINTS2D_H

#include "renderable.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QColor>
class Simulator;

class Points2D;
class Points2DRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    Points2DRenderer();
private:
    virtual void synchronize(Renderable *renderable) override;
    virtual void render() override;

    void uploadVBO(Points2D* points);
    void beforeLinkProgram() override;
    int m_vertexCount = 0;
    float m_pointSize = 1.0;
    QVector4D m_color = QVector4D(1.0, 0.0, 0.0, 0.0);
    bool m_isInitialized = false;
};

class Points2D : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float pointSize READ pointSize WRITE setPointSize NOTIFY pointSizeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Points2D();
    ~Points2D();
    void setPositions(QVector<QVector2D> &positions);
    QVector<QVector2D> &positions();
    virtual RenderableRenderer* createRenderer();

    float pointSize() const
    {
        return m_pointSize;
    }

    QColor color() const
    {
        return m_color;
    }

public slots:

    void setPointSize(float arg)
    {
        if (m_pointSize == arg)
            return;

        m_pointSize = arg;
        emit pointSizeChanged(arg);
    }

    void setColor(QColor arg)
    {
        if (m_color == arg)
            return;

        m_color = arg;
        emit colorChanged(arg);
    }

signals:

    void pointSizeChanged(float arg);

    void colorChanged(QColor arg);

private:
    QVector<QVector2D> m_vertices;
    float m_pointSize = 1.0;
    QColor m_color = "red";
    friend class Points2DRenderer;
};


#endif // POINTS2D_H
