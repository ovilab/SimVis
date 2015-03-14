#include "mysimulator.h"

#include "billboards2d.h"

MySimulator::MySimulator()
{

}

MySimulator::~MySimulator()
{

}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

MyWorker::MyWorker()
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

}
