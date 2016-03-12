#ifndef MARCHINGCUBESGS_H
#define MARCHINGCUBESGS_H

#include "../../core/renderable.h"
#include <QOpenGLTexture>
class MarchingCubesGSRenderer : public RenderableRenderer
{
    Q_OBJECT
private:
    int m_voxelsPerDimension = 64;
    int m_numberOfVoxels = 0;
    float m_scale = 10;
    float m_threshold = 0.0;
    void uploadVBO();
    bool m_isInitialized = false;
    GLint *m_triangleTable = nullptr;
    QOpenGLTexture *m_triangleTableTexture = nullptr;
    void createTriangleTable();
    // RenderableRenderer interface
private:
    virtual void beforeLinkProgram();
    virtual void synchronize(Renderable *renderable);
    virtual void render();
};

class MarchingCubesGS : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
private:
    float m_threshold = 0.0;
public:
    virtual RenderableRenderer *createRenderer();
    float threshold() const;
public slots:
    void setThreshold(float threshold);
signals:
    void thresholdChanged(float threshold);
};

#endif // MARCHINGCUBESGS_H
