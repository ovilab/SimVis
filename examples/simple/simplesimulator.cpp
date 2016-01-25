#include "simplesimulator.h"
#include <QVector3D>
#include <QVector2D>

SimulatorWorker *SimpleSimulator::createWorker()
{
    return new SimpleWorker();
}

SimpleWorker::SimpleWorker()
{
    int ballCount = 100;
    m_positions.resize(ballCount);
    m_velocities.resize(ballCount);
    for(auto i=0; i < ballCount; i++) {
        float x =  12.0*(rand() / double(RAND_MAX)) - 6.0;
        float y =  12.0*(rand() / double(RAND_MAX)) - 6.0;
        float z =  12.0*(rand() / double(RAND_MAX)) - 6.0;
        float vx = 12.0*(rand() / double(RAND_MAX)) - 6.0;
        float vy = 12.0*(rand() / double(RAND_MAX)) - 6.0;
        float vz = 12.0*(rand() / double(RAND_MAX)) - 6.0;

        m_positions[i] = 0.5 * QVector3D(x, y, z);
        m_velocities[i] = 0.3 * QVector3D(vx, vy, vz);
    }
}

void SimpleWorker::synchronizeSimulator(Simulator *simulator)
{
    SimpleSimulator *sim = static_cast<SimpleSimulator *>(simulator);
    Q_UNUSED(sim)
    // ... synchronize worker and simulator here
}

void SimpleWorker::work()
{
    float m = 2.0;
    float k = 0.1;
    float dt = 0.01;
    for(int i = 0; i < m_positions.size(); i++) {
        QVector3D F;
        QVector3D a;
        QVector3D &r = m_positions[i];
        QVector3D &v = m_velocities[i];

        F = -r*k;
        a = F / m;
        v += a*dt;
        r += v*dt;
    }
}
