#include "bondssimulator.h"
#include <QDebug>
#include <SimVis/Spheres>
#include <renderables/bonds/bonds.h>
MySimulator::MySimulator()
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
    using namespace SimVis;
    m_positions.resize(5);
    m_velocities.resize(m_positions.size());
    double size = 10;
    for(int i=0; i<m_positions.size(); i++) {
        float x = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        float y = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        float z = ((2.0*rand() / double(RAND_MAX))-1.0)*size;
        x = i*10;
        y = 0;
        z = 0;
        float vx = ((2.0*rand() / double(RAND_MAX))-1.0)*0.01;
        float vy = ((2.0*rand() / double(RAND_MAX))-1.0)*0.01;
        float vz = ((2.0*rand() / double(RAND_MAX))-1.0)*0.01;
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
    }
}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    // Synchronize with renderables.
    Spheres *spheres = qobject_cast<Spheres*>(renderableObject);
    if(spheres) {
        spheres->positions() = m_positions;
        spheres->colors().resize(m_positions.size());
        spheres->scales().resize(m_positions.size());
        for(int i=0; i<m_positions.size(); i++) {
            spheres->colors()[i] = QColor("red");
            spheres->scales()[i] = 2.0;
        }

        spheres->setDirty(true);
        return;
    }

    Bonds *bonds = qobject_cast<Bonds*>(renderableObject);
    if(bonds) {
        QVector<BondsVBOData> &vbo = bonds->vertices();
        vbo.clear();
        for(int i=0; i<m_positions.size(); i++) {
            QVector3D position_i = m_positions[i];
            for(int j=i+1; j<m_positions.size(); j++) {
                QVector3D position_j = m_positions[j];
                float dist = (position_i - position_j).length();
                if(dist < 20) {
                    for(int k=0; k<6; k++) {
                        BondsVBOData data;
                        data.radius1 = 0.2;
                        data.radius2 = 0.2;
                        data.sphereRadius1 = 1.0*0.9;
                        data.sphereRadius2 = 1.0*0.9;
                        data.vertex1Position = position_i;
                        data.vertex2Position = position_j;
                        data.vertexId = k;
                        vbo.push_back(data);
                    }
                }
            }
        }

        qDebug() << "We have " << vbo.size() / 6 << " bonds";
        bonds->setDirty(true);
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
