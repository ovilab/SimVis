#ifndef MARCHINGCUBESGS_H
#define MARCHINGCUBESGS_H

#include "../../core/renderable.h"
#include <QOpenGLTexture>
class MarchingCubesGSRenderer : public RenderableRenderer
{
    Q_OBJECT
private:
    int m_voxelsPerDimension = 32;
    int m_numberOfVoxels = 0;
    float m_scale = 16;
    float m_threshold = 0.0;
    bool m_dirty = true;
    void uploadVBO();
    bool m_isInitialized = false;
    GLint *m_triangleTable = nullptr;
    QOpenGLTexture *m_triangleTableTexture = nullptr;
    void createTriangleTable();
    // RenderableRenderer interface
private:
    virtual void synchronize(Renderable *renderable);
    virtual void render();
};

class MarchingCubesGS : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(int resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
private:
    float m_threshold = 0.0;
    int m_resolution = 32;
    float m_scale = 10.0;

public:
    virtual RenderableRenderer *createRenderer();
    float threshold() const;
    int resolution() const;
    float scale() const;

public slots:
    void setThreshold(float threshold);
    void setResolution(int resolution);
    void setScale(float scale);

signals:
    void thresholdChanged(float threshold);
    void resolutionChanged(int resolution);
    void scaleChanged(float scale);
};

#endif // MARCHINGCUBESGS_H
