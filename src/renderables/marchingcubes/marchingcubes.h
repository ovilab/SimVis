#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "../../core/renderable.h"
#include "marchingcubesgenerator.h"

class MarchingCubesRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    MarchingCubesRenderer();
protected:
    MarchingCubesGenerator m_generator;
    unsigned int m_indexCount = 0;
private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;

    void uploadVBOs();

    virtual void beforeLinkProgram() override;
    bool m_isInitialized = false;
};

class MarchingCubes : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QVector3D min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(QVector3D max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QVector3D numVoxels READ numVoxels WRITE setNumVoxels NOTIFY numVoxelsChanged)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
protected:
    float m_threshold;
    QVector3D m_numVoxels;
    QVector3D m_min;
    QVector3D m_max;
    bool m_dirty;
    bool m_hasScalarField = false;
    function<float(const QVector3D point)> m_scalarFieldEvaluator;

public:
    MarchingCubes();
    ~MarchingCubes();

    RenderableRenderer *createRenderer();
    float threshold() const;
    QVector3D min() const;
    QVector3D max() const;
    bool dirty() const;
    QVector3D numVoxels() const;
    bool hasScalarField() const;
    function<float (const QVector3D point)> scalarFieldEvaluator() const;
    void setScalarFieldEvaluator(const function<float (const QVector3D point)> &scalarFieldEvaluator);

public slots:
    void setThreshold(float arg);
    void setMin(QVector3D arg);
    void setMax(QVector3D arg);
    void setDirty(bool arg);
    void setNumVoxels(QVector3D arg);

signals:
    void thresholdChanged(float arg);
    void minChanged(QVector3D arg);
    void maxChanged(QVector3D arg);
    void dirtyChanged(bool arg);
    void numVoxelsChanged(QVector3D arg);
};

#endif // MARCHINGCUBES_H
