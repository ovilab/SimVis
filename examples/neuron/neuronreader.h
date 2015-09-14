#ifndef NEURONREADER_H
#define NEURONREADER_H

#include "core/renderable.h"

#include <SimVis/QuickWorker>
#include <SimVis/Simulator>
#include <QObject>
#include <QUrl>
#include <QVector3D>

class NeuronReader;
class NeuronRenderable;

struct SphereVBOData
{
    float sphereId;
    QVector3D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Segment {
public:
    int id = -1;
    int parentID = -1;
    Segment* parent;
    QVector3D proximal;
    QVector3D distal;
    double proximalWidth = 0.0;
    double distalWidth = 0.0;
    bool hasParentID = false;
    bool hasProximal = false;
    bool hasDistal = false;
};

class NeuronRenderer : public RenderableRenderer
{
public:
    NeuronRenderer();
private:
    virtual void beforeLinkProgram();
    virtual void synchronize(Renderable *renderable);
    virtual void render();

    void uploadVBOs(NeuronRenderable *neuron);

    bool m_isInitialized = false;
    int m_vertexCount = 0;
    int m_indexCount = 0;
    QVector3D m_upVector;
    QVector3D m_viewVector;
    QVector3D m_rightVector;
};

class NeuronRenderable : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    virtual RenderableRenderer *createRenderer();
    const QColor &color() const;
public slots:
    void setColor(QColor arg);

signals:
    void colorChanged(QColor arg);

private:
    QVector<Segment> m_segments;
    QVector<SphereVBOData> m_vertices;
    QVector<GLuint> m_indices;
    QColor m_color = QColor(0.8, 0.7, 0.5, 1.0);
    QVector<QVector3D> m_positions;
    float m_scale = 10.0;
    QVector3D vectorFromColor(const QColor &color);

    friend class NeuronWorker;
    friend class NeuronRenderer;
};

class NeuronWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    NeuronWorker();

private:
    QVector<Segment> m_segments;
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void synchronizeRenderer(Renderable *renderable) override;
    virtual void work() override;
    void reset();
};

class NeuronReader : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
public:
    explicit NeuronReader(QQuickItem *parent = 0);
    ~NeuronReader();

    QUrl source() const;

    void readFile();

signals:

    void sourceChanged(QUrl arg);

public slots:

    void setSource(QUrl arg);

    // Simulator interface
protected:
    SimulatorWorker *createWorker();

private:
    QUrl m_source;
    QVector<Segment> m_segments;
    bool m_segmentsAreDirty = true;

    friend class NeuronWorker;
};

#endif // NEURONREADER_H
