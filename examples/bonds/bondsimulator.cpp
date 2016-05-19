#include "bondsimulator.h"
#include <QDebug>
#include <SimVis/SphereData>
#include <SimVis/BondData>

BondSimulator::BondSimulator(QNode *parent)
    : Simulator(parent)
    , m_sphereData(new SphereData(this))
    , m_bondData(new BondData(this))
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
    m_positions.push_back(QVector3D(4, 0, 0));
    m_positions.push_back(QVector3D(-4, -2, 0));

    BondVBOData bond;
    QVector3D diff = m_positions[1] - m_positions[0];
    bond.vertex1 = m_positions[0];
    bond.vertex2 = m_positions[1];
    bond.radius1 = 0.2;
    bond.radius2 = 0.2;
    bond.sphereRadius1 = 0.5;
    bond.sphereRadius2 = 0.5;

    m_bonds.push_back(bond);
}

void BondWorker::synchronizeSimulator(Simulator *simulator)
{
    BondSimulator *mySimulator = qobject_cast<BondSimulator*>(simulator);
    if(mySimulator) {
        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
        dt = mySimulator->dt();

        mySimulator->sphereData()->setPositions(m_positions);
        mySimulator->bondData()->setData(m_bonds);
    }
}

void BondWorker::work()
{
}
