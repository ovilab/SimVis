#include "mysimulator.h"
#include "billboards2d.h"

#include <QVector2D>

MySimulator::MySimulator(QObject *parent) :
    Simulator(parent)
{

}

MySimulator::~MySimulator()
{

}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker(QObject *parent) :
    SimulatorWorker(parent)
{
    unsigned int numberOfBalls = 100;
    for(unsigned int i=0; i<numberOfBalls; i++) {
        float x =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float y =  2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vx = 2.0*(rand() / double(RAND_MAX)) - 1.0;
        float vy = 2.0*(rand() / double(RAND_MAX)) - 1.0;

        x *= 0.5;
        y *= 0.5;
        vx *= 0.3;
        vy *= 0.3;

        m_positions.push_back(QVector2D(x,y));
        m_velocities.push_back(QVector2D(vx,vy));
    }
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{

}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    Billboards2D* billboards = qobject_cast<Billboards2D*>(renderableObject);
    if(billboards) {
        billboards->setPositions(m_positions);
    }
}

void MyWorker::work()
{
    float springConstant = 1.0;
    float mass = 1.0;
    float dt = 0.01;
    for(unsigned int i=0; i<m_positions.size(); i++) {
        QVector2D force = -m_positions[i]*springConstant;
        m_velocities[i] += force/mass*dt;
        m_positions[i] += m_velocities[i]*dt;
    }
}
