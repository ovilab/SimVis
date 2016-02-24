#ifndef CYLINDERS_H
#define CYLINDERS_H

#include "../../core/renderable.h"
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QColor>

class Simulator;

struct CylinderVBOData
{
    QVector3D vertex1;
    QVector3D vertex2;
    float radius1;
    float radius2;
};

class Cylinders;
class CylindersRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    CylindersRenderer();

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;
    virtual void beforeLinkProgram() override;

    float m_radius = 0.1;
    void uploadVBOs(Cylinders* Cylinders);
    bool m_isInitialized = false;
    int m_vertexCount = 0;
    int m_indexCount = 0;
    void geometryShaderMissingError();
    bool m_hasPrintedError = false;
};

class Cylinders : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
public:
    Cylinders(QQuickItem *parent = 0);
    ~Cylinders();

    virtual CylindersRenderer *createRenderer();
    bool dirty() const;
    QVector<CylinderVBOData> &cylinders() { return m_cylinders; }

    float radius() const;
    void setCylinders(const QVector<CylinderVBOData> &cylinders);

public slots:
    void setDirty(bool dirty);
    void setRadius(float radius);

signals:
    void dirtyChanged(bool dirty);
    void radiusChanged(float radius);

private:
    QVector<CylinderVBOData> m_cylinders;

    friend class CylindersRenderer;
    bool m_dirty = false;
    float m_radius = 0.025;
};


#endif // CYLINDERS_H
