#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/TriangleCollection>

#include <QVector>
#include <QVector3D>

#include <Qt3DRender/QBuffer>

#include "spheredata.h"

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();

private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void synchronizeRenderer(Renderable *renderableObject);
    virtual void work();
    QVector<QVector3D> m_positions;
    QVector<QVector3D> m_velocities;
    float dt = 0.05;
    QElapsedTimer m_timer;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)

public:
    MySimulator(QNode *parent = 0);
    virtual ~MySimulator();

    double dt() const;
    SphereData* sphereData()
    {
        return m_sphereData;
    }

public slots:
    void setDt(double dt);

signals:
    void dtChanged(double dt);

protected:
    virtual SimulatorWorker *createWorker();

private:
    double m_dt = 0.05;
    SphereData *m_sphereData = nullptr;

    QT3D_CLONEABLE(MySimulator)

};

#endif // MYSIMULATOR_H
