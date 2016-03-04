#include "cylindersimulator.h"
#include <QDebug>
#include <SimVis/Spheres>

CylinderSimulator::CylinderSimulator()
{

}

SimulatorWorker *CylinderSimulator::createWorker()
{
    return new CylinderWorker();
}

CylinderWorker::CylinderWorker()
{
    m_cylinders.resize(100);
    double spread = 30.0;
    for(CylinderVBOData &cylinder : m_cylinders) {
        float x = ((2.0*rand() / double(RAND_MAX))-1.0)*spread;
        float y = ((2.0*rand() / double(RAND_MAX))-1.0)*spread;
        float z = ((2.0*rand() / double(RAND_MAX))-1.0)*spread;

        QVector3D direction(((2.0*rand() / double(RAND_MAX))-1.0),
                               ((2.0*rand() / double(RAND_MAX))-1.0),
                               ((2.0*rand() / double(RAND_MAX))-1.0));
        QVector3D position(x, y, z);
        double length = 1.0 + 8.0*rand() / double(RAND_MAX);

        cylinder.radius1 = 1.0 + rand() / double(RAND_MAX);
        cylinder.radius2 = 1.0 + rand() / double(RAND_MAX);
        cylinder.vertex1 = position + direction * length * 0.5;
        cylinder.vertex2 = position - direction * length * 0.5;
    }

}

void CylinderWorker::synchronizeSimulator(Simulator *simulator)
{
    CylinderSimulator *cylinderSimulator = qobject_cast<CylinderSimulator*>(simulator);
    if(cylinderSimulator) {
        // Synchronize data between QML thread and computing thread (CylinderSimulator is on QML, CylinderWorker is computing thread).
        // This is for instance data from user through GUI (sliders, buttons, text fields etc)
    }
}

void CylinderWorker::synchronizeRenderer(Renderable *renderableObject)
{
    // Synchronize with renderables.
    Cylinders *cylinders = qobject_cast<Cylinders*>(renderableObject);
    if(cylinders) {
        cylinders->setCylinders(m_cylinders);
        cylinders->setDirty(true);
        return;
    }
}

void CylinderWorker::work()
{
//    double dt = 0.01;
//    for(int i=0; i<m_positions.size(); i++) {
//        float ax = ((2.0*rand() / double(RAND_MAX))-1.0);
//        float ay = ((2.0*rand() / double(RAND_MAX))-1.0);
//        float az = ((2.0*rand() / double(RAND_MAX))-1.0);
//        m_velocities[i][0] += ax*dt;
//        m_velocities[i][1] += ay*dt;
//        m_velocities[i][2] += az*dt;

//        m_positions[i] += m_velocities[i]*dt;
//    }
}
