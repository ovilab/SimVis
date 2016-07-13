#ifndef BONDS_H
#define BONDS_H

#include "../../core/renderable.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QColor>

class Simulator;

struct BondsVBOData
{
    QVector3D vertex1Position;
    QVector3D vertex2Position;
    float radius1;
    float radius2;
    float sphereRadius1;
    float sphereRadius2;
    float vertexId;
};

class BondsRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    BondsRenderer();

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;
    virtual void beforeLinkProgram() override;

    void uploadVBOs(class Bonds* bonds);
    bool m_isInitialized = false;
    int m_vertexCount = 0;
    int m_indexCount = 0;
};

class Bonds : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    Bonds(QQuickItem *parent = 0);
    ~Bonds();

    virtual BondsRenderer *createRenderer() override;
    bool dirty() const;
    QVector<BondsVBOData> &vertices();
    float radius() const;
    QColor color() const;

public slots:
    void setDirty(bool dirty);
    void setRadius(float radius);
    void setColor(QColor color);

signals:
    void dirtyChanged(bool dirty);
    void radiusChanged(float radius);
    void colorChanged(QColor color);

private:
    QVector<BondsVBOData> m_vertices;
    bool m_dirty = false;
    float m_radius = 0.01f;
    QColor m_color = "white";
    friend class BondsRenderer;
};


#endif // BONDS_H
