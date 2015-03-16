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

int MySimulator::numberOfBalls() const
{
    return m_numberOfBalls;
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

void MySimulator::setNumberOfBalls(int arg)
{
    if (m_numberOfBalls == arg)
        return;

    m_numberOfBalls = arg;
    emit numberOfBallsChanged(arg);
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
    MySimulator *sim = static_cast<MySimulator *>(simulator);
    m_dt = sim->dt();
    m_springConstant = sim->springConstant();
    m_mass = sim->mass();
    m_numberOfBalls = sim->numberOfBalls();
    if(sim->m_willReset) {
        reset();
        sim->m_willReset = false;
    }
}

void MyWorker::work()
{
    float oneOverMass = 1.0/m_mass;
    for(auto i=0; i<m_positions.size(); i++) {
        QVector2D force;
        force[0] = -m_positions[i][0]*m_springConstant;
        force[1] = -m_positions[i][1]*m_springConstant;
        m_velocities[i][0] += force[0]*oneOverMass*m_dt;
        m_velocities[i][1] += force[1]*oneOverMass*m_dt;

        m_positions[i][0] += m_velocities[i][0]*m_dt;
        m_positions[i][1] += m_velocities[i][1]*m_dt;
    }
}

void MyWorker::reset()
{
    m_positions.clear();
    m_velocities.clear();
    m_positions.resize(m_numberOfBalls);
    m_velocities.resize(m_numberOfBalls);
    for(auto i=0; i<m_numberOfBalls; i++) {
        float x =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float y =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vx = 2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vy = 2.0*(rand() / double(RAND_MAX)) - 1.0;

        x *= 0.5;
        y *= 0.5;
        vx *= 0.3;
        vy *= 0.3;

        m_positions[i] = QVector3D(x,y, 0);
        m_velocities[i] = QVector2D(vx,vy);
    }
}
