#include "mysimulator.h"

#include <QVector3D>

MySimulator::MySimulator()
{

}

MySimulator::~MySimulator()
{

}

float MySimulator::springConstant() const
{
    return m_springConstant;
}

float MySimulator::mass() const
{
    return m_mass;
}

float MySimulator::dt() const
{
    return m_dt;
}

int MySimulator::ballCount() const
{
    return m_numberOfBalls;
}

QVector3D MySimulator::firstParticlePosition() const
{
    return m_firstParticlePosition;
}

void MySimulator::setSpringConstant(float arg)
{
    if (m_springConstant == arg)
        return;

    m_springConstant = arg;
    emit springConstantChanged(arg);
}

void MySimulator::setMass(float arg)
{
    if (m_mass == arg)
        return;

    m_mass = arg;
    emit massChanged(arg);
}

void MySimulator::setDt(float arg)
{
    if (m_dt == arg)
        return;

    m_dt = arg;
    emit dtChanged(arg);
}

void MySimulator::reset()
{
    m_willReset = true;
}

void MySimulator::setBallCount(int arg)
{
    if (m_numberOfBalls == arg)
        return;

    m_numberOfBalls = arg;
    emit ballCountChanged(arg);
}

void MySimulator::setFirstParticlePosition(QVector3D arg)
{
    if (m_firstParticlePosition == arg)
        return;

    m_firstParticlePosition = arg;
    emit firstParticlePositionChanged(arg);
}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker()
{
    reset();
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    MySimulator *mySimulator = static_cast<MySimulator *>(simulator);
    m_dt = mySimulator->dt();
    m_springConstant = mySimulator->springConstant();
    m_mass = mySimulator->mass();
    m_ballCount = mySimulator->ballCount();
    if(mySimulator->m_willReset) {
        reset();
        mySimulator->m_willReset = false;
    }
    if(m_positions.size() > 0) {
        mySimulator->setFirstParticlePosition(m_positions[0]);
    }
}

void MyWorker::work()
{
    float m = m_mass;
    float k = m_springConstant;
    float dt = m_dt;
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

void MyWorker::reset()
{
    m_positions.clear();
    m_velocities.clear();
    m_positions.resize(m_ballCount);
    m_velocities.resize(m_ballCount);
    for(auto i=0; i < m_ballCount; i++) {
        float x =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float y =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float z =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vx = 2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vy = 2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vz = 2.0*(rand() / double(RAND_MAX)) - 1.0;

        // x *= 5; y *= 5; z *= 5; vx *= 5; vy *= 5; vz *= 5;

        m_positions[i] = 0.7 * QVector3D(x, y, z);
        m_velocities[i] = 0.4 * QVector3D(vx, vy, vz);
    }
}
