#include "spheressimulator.h"
#include <QDebug>
#include <SimVis/Spheres>
MySimulator::MySimulator()
    : m_positionBuffer(new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, 0))
{
}

double MySimulator::dt() const
{
    return m_dt;
}

void MySimulator::setDt(double dt)
{
    if (m_dt == dt)
        return;

    m_dt = dt;
    emit dtChanged(dt);
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker()
{
    m_positions.resize(100);
    m_velocities.resize(m_positions.size());
    double size = 10;
    for(int i=0; i<m_positions.size(); i++) {
        float x = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        float y = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        float z = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        float vx = ((2.0*rand() / double(RAND_MAX))-1.0);
        float vy = ((2.0*rand() / double(RAND_MAX))-1.0);
        float vz = ((2.0*rand() / double(RAND_MAX))-1.0);
        m_positions[i] = QVector3D(x,y,z);
        m_velocities[i] = QVector3D(vx,vy,vz);
    }
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = qobject_cast<MySimulator*>(simulator);
    if(mySimulator) {
        // Synchronize data between QML thread and computing thread (MySimulator is on QML, MyWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
        dt = mySimulator->dt();

        QByteArray ba;
        ba.resize(m_positions.size() * sizeof(QVector3D));

        QVector3D *posData = reinterpret_cast<QVector3D *>(ba.data());
        for(const QVector3D &pos : m_positions) {
            *posData = pos;
            ++posData;
        }
        mySimulator->positionBuffer()->setData(ba);
    }
}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    // Synchronize with renderables.
    Spheres *spheres = qobject_cast<Spheres*>(renderableObject);
    if(spheres) {
        spheres->positions() = m_positions;
        spheres->triggerDirtyData();
        return;
    }
}

void MyWorker::work()
{
    for(int i=0; i<m_positions.size(); i++) {
        float ax = ((2.0*rand() / double(RAND_MAX))-1.0);
        float ay = ((2.0*rand() / double(RAND_MAX))-1.0);
        float az = ((2.0*rand() / double(RAND_MAX))-1.0);
        m_velocities[i][0] += ax*dt;
        m_velocities[i][1] += ay*dt;
        m_velocities[i][2] += az*dt;

        m_positions[i] += m_velocities[i]*dt;
    }
}
