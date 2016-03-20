#ifndef CylinderSimulator_H
#define CylinderSimulator_H

#include <SimVis/Simulator>
#include <SimVis/Cylinders>

#include <QVector>
#include <QVector3D>

class CylinderWorker : public SimulatorWorker
{
    Q_OBJECT
private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void synchronizeRenderer(Renderable *renderableObject);
    virtual void work();
    QVector<CylinderVBOData> m_cylinders;
public:
    CylinderWorker();
};

class CylinderSimulator : public Simulator
{
    Q_OBJECT
public:
    CylinderSimulator();

protected:
    virtual SimulatorWorker *createWorker();

    QT3D_CLONEABLE(CylinderSimulator)
};

#endif // CylinderSimulator_H
