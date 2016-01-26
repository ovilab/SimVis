#ifndef NOISEVIEWER_H
#define NOISEVIEWER_H

#include "../../core/renderable.h"

class NoiseViewerRenderer : public RenderableRenderer
{
    Q_OBJECT
private:
    int m_voxelsPerDimension = 64;
    int m_numberOfVoxels = 0;
    float m_scale = 1;
    void uploadVBO();
    bool m_isInitialized = false;
    // RenderableRenderer interface
private:
    virtual void beforeLinkProgram();
    virtual void synchronize(Renderable *renderable);
    virtual void render();
};

class NoiseViewer : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(int voxelsPerDimension READ voxelsPerDimension WRITE setVoxelsPerDimension NOTIFY voxelsPerDimensionChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
private:
    int m_voxelsPerDimension = 64;
    float m_scale = 1;

public:
    virtual RenderableRenderer *createRenderer();
    int voxelsPerDimension() const;
    float scale() const;

public slots:
    void setVoxelsPerDimension(int voxelsPerDimension);
    void setScale(float scale);

signals:
    void voxelsPerDimensionChanged(int voxelsPerDimension);
    void scaleChanged(float scale);
};

#endif // NOISEVIEWER_H
