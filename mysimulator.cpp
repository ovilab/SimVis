#include "mysimulator.h"

MySimulator::MySimulator()
{

}

MySimulator::~MySimulator()
{

}

Worker *MySimulator::createWorker()
{
    return new MyWorker();
}



void MyWorker::synchronizeSimulator(Simulator *simulator)
{

}

void MyWorker::synchronizeRenderer(RenderableObject *renderableObject)
{

}

void MyWorker::work()
{

}
