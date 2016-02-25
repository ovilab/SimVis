#ifndef NEURONREADER_H
#define NEURONREADER_H

#include "core/renderable.h"

#include <SimVis/QuickWorker>
#include <SimVis/Simulator>
#include <SimVis/Cylinders>
#include <SimVis/Spheres>
#include <QObject>
#include <QUrl>
#include <QVector3D>

class NeuronReader;
class NeuronRenderable;

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

class NeuronWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    NeuronWorker();

private:
    QVector<Segment> m_segments;
    QVector<CylinderVBOData> m_cylinders;
    QVector<QVector3D> m_spheres;
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
    QVector<CylinderVBOData> m_cylinders;
    QVector<QVector3D> m_spheres;
    bool m_segmentsAreDirty = true;

    friend class NeuronWorker;
};

#endif // NEURONREADER_H
