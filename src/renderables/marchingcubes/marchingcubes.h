#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "../../core/renderable.h"
#include "marchingcubesgenerator.h"
#include <QColor>
class MarchingCubesRenderer;

class MarchingCubes : public Renderable
{
    Q_OBJECT
    Q_ENUMS(Mode)
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QVector3D min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(QVector3D max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QVector3D numVoxels READ numVoxels WRITE setNumVoxels NOTIFY numVoxelsChanged)
    Q_PROPERTY(bool dirty READ dirty WRITE setDirty NOTIFY dirtyChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float alpha READ alpha WRITE setAlpha NOTIFY alphaChanged)
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

public:
    enum Mode
    {
        FRONT_AND_BACK,
        FRONT,
        BACK,
        LINES
    };

    MarchingCubes();
    ~MarchingCubes();

    RenderableRenderer *createRenderer();
    float threshold() const;
    QVector3D min() const;
    QVector3D max() const;
    bool dirty() const;
    QVector3D numVoxels() const;
    bool hasScalarField() const;
    bool scalarFieldDirty() const;
    void setScalarFieldDirty(bool scalarFieldDirty);
    function<float (const QVector3D point)> scalarFieldEvaluator() const;
    void setScalarFieldEvaluator(const function<float (const QVector3D point)> &scalarFieldEvaluator);
    function<QVector4D (const QVector3D point)> colorEvaluator() const;
    void setColorEvaluator(const function<QVector4D(const QVector3D)> &colorEvaluator);
    QColor color() const;
    Mode mode() const;
    float alpha() const;

public slots:
    void setThreshold(float arg);
    void setMin(QVector3D arg);
    void setMax(QVector3D arg);
    void setDirty(bool arg);
    void setNumVoxels(QVector3D arg);
    void setColor(QColor arg);
    void setMode(Mode arg);
    void setAlpha(float arg);

signals:
    void thresholdChanged(float arg);
    void minChanged(QVector3D arg);
    void maxChanged(QVector3D arg);
    void dirtyChanged(bool arg);
    void numVoxelsChanged(QVector3D arg);
    void colorChanged(QColor arg);
    void modeChanged(Mode arg);

    void alphaChanged(float arg);

private:
    float m_threshold = 0.0;
    QVector3D m_numVoxels;
    QVector3D m_min;
    QVector3D m_max;
    QColor m_color;
    Mode m_mode = FRONT_AND_BACK;
    float m_alpha = 1.0;
    bool m_dirty = false;
    bool m_scalarFieldDirty = false;
    bool m_hasScalarField = false;
    function<float(const QVector3D point)> m_scalarFieldEvaluator;
    function<QVector4D(const QVector3D point)> m_colorEvaluator = 0;

    friend class MarchingCubesRenderer;
};

class MarchingCubesRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    MarchingCubesRenderer();
protected:
    MarchingCubesGenerator m_generator;
    unsigned int m_triangleIndexCount = 0;
    unsigned int m_lineIndexCount = 0;
    QVector4D m_color;
    float m_alpha = 1.0;
    MarchingCubes::Mode m_mode = MarchingCubes::FRONT_AND_BACK;

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;

    void uploadVBOs();

    virtual void beforeLinkProgram() override;
    bool m_isInitialized = false;
};

#endif // MARCHINGCUBES_H
