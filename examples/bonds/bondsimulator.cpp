#include "bondsimulator.h"
#include <QDebug>
#include <SimVis/SphereData>
#include <SimVis/CylinderData>
BondSimulator::BondSimulator(QNode *parent)
    : Simulator(parent)
    , m_sphereData(new SphereData(this))
    , m_cylinderData(new CylinderData(this))
{
}

double BondSimulator::dt() const
{
    return m_dt;
}

void BondSimulator::setDt(double dt)
{
    if (m_dt == dt)
        return;

    m_dt = dt;
    emit dtChanged(dt);
}

SimulatorWorker *BondSimulator::createWorker()
{
    return new BondWorker();
}

BondWorker::BondWorker()
{
    m_positions.push_back(QVector3D(2, 2, 0));
    m_positions.push_back(QVector3D(-2, -2, 0));

    CylinderVBOData cylinder;
    QVector3D diff = m_positions[1] - m_positions[0];
    cylinder.vertex1 = m_positions[0] + diff.normalized() * 0.5;
    cylinder.vertex2 = m_positions[1] - diff.normalized() * 0.5;
    cylinder.radius1 = 0.2;
    cylinder.radius2 = 0.2;

    m_cylinders.push_back(cylinder);
}

void BondWorker::synchronizeSimulator(Simulator *simulator)
{
    BondSimulator *mySimulator = qobject_cast<BondSimulator*>(simulator);
    if(mySimulator) {
        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
        dt = mySimulator->dt();

        mySimulator->sphereData()->setPositions(m_positions);
        mySimulator->cylinderData()->setData(m_cylinders);
    }
}

void BondWorker::work()
{
}
