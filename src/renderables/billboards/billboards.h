#pragma once
#ifndef BILLBOARDS_H
#define BILLBOARDS_H

#include "../../core/renderable.h"

#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QColor>

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
    QVector3D m_upVector;
    QVector3D m_viewVector;
    QVector3D m_rightVector;
    QColor m_shadowColor;
};

class Billboards : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QString texture READ texture WRITE setTexture NOTIFY textureChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
public:
    Billboards(QObject *parent = 0);
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
    QColor shadowColor() const;

public slots:
    void setTexture(QString arg);
    void setShadowColor(QColor arg);

signals:
    void scaleChanged(bool arg);
    void textureChanged(QString arg);

    void shadowColorChanged(QColor arg);

private:
    QVector<BillboardVBOData> m_vertices;
    QVector<GLuint> m_indices;
    QVector3D m_color = QVector3D(1.0, 1.0, 1.0);

    QVector<QVector3D> m_positions;
    QVector<float> m_rotations;
    float m_scale = 1.0;

    QVector3D vectorFromColor(const QColor &color);

    friend class BillboardsRenderer;
    QString m_texture = "NO TEXTURE CHOSEN";
    QColor m_shadowColor = QColor(0, 0, 0, 255);
};


#endif // BILLBOARDS_H
