#pragma once
#ifndef BILLBOARDS_H
#define BILLBOARDS_H

#include "renderable.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class Simulator;

struct BillboardVBOData
{
    QVector3D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Billboards;
class BillboardsRenderer : public RenderableRenderer
{
public:
    BillboardsRenderer();

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;

    void uploadVBOs(Billboards* billboards);

    virtual void beforeLinkProgram() override;
    void uploadTexture(QString filename);
    QOpenGLTexture *m_texture = 0;
    bool m_isInitialized = false;
    bool m_isTextureUploaded = false;
    int m_vertexCount;
    int m_indexCount;
};

class Billboards : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString texture READ texture WRITE setTexture NOTIFY textureChanged)
public:
    Billboards();
    ~Billboards();
    void setPositions(QVector<QVector3D> &positions);
    QVector<QVector3D> &positions();
    QVector<float> &rotations();

    float scale() const;
    void setScale(float scale);

    QVector3D color() const;
    void setColor(const QColor &color);

    virtual RenderableRenderer* createRenderer();

    QString texture() const;

public slots:
    void setTexture(QString arg);

signals:
    void scaleChanged(bool arg);
    void textureChanged(QString arg);

private:
    QVector<BillboardVBOData> m_vertices;
    QVector<GLuint> m_indices;
    QVector3D m_color;

    QVector<QVector3D> m_positions;
    QVector<float> m_rotations;
    float m_scale = 1.0;

    QVector3D vectorFromColor(const QColor &color);

    friend class BillboardsRenderer;
    QString m_texture = "NO TEXTURE CHOSEN";
};


#endif // BILLBOARDS_H
