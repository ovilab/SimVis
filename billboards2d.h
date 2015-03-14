#pragma once
#ifndef BILLBOARDS2D_H
#define BILLBOARDS2D_H

#include "renderable.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>
using std::vector;

class Simulator;

struct Billboard2DVBOData
{
    QVector2D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Billboards2D;
class Billboards2DRenderer : public RenderableRenderer
{
public:
    Billboards2DRenderer();

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;

    void uploadVBOs(Billboards2D* billboards);

    virtual void beforeLinkProgram() override;
    void uploadTexture(QString filename);
    QOpenGLTexture *m_texture = 0;
    bool m_isInitialized = false;
    bool m_isTextureUploaded = false;
    int m_vertexCount;
    int m_indexCount;
};

class Billboards2D : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString texture READ texture WRITE setTexture NOTIFY textureChanged)
public:
    Billboards2D();
    ~Billboards2D();
    void setPositions(std::vector<QVector2D> &positions);
    std::vector<QVector2D> &positions();
    std::vector<float> &rotations();

    float scale() const;
    void setScale(float scale);

    QVector3D color() const;
    void setColor(const QColor &color);

    virtual RenderableRenderer* createRenderer();

    QString texture() const
    {
        return m_texture;
    }

public slots:
    void setTexture(QString arg)
    {
        if (m_texture == arg)
            return;

        m_texture = arg;
        emit textureChanged(arg);
    }

signals:
    void scaleChanged(bool arg);
    void textureChanged(QString arg);

private:
    std::vector<Billboard2DVBOData> m_vertices;
    std::vector<GLuint> m_indices;
    QVector3D m_color;

    std::vector<QVector2D> m_positions;
    std::vector<float> m_rotations;
    float m_scale = 1.0;

    QVector3D vectorFromColor(const QColor &color);

    friend class Billboards2DRenderer;
    QString m_texture = "NO TEXTURE CHOSEN";
};


#endif // BILLBOARDS2D_H
