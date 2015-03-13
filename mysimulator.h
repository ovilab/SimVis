#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "simulator.h"
#include <QDebug>

class MyWorker : public Worker {
public:
    void synchronizeSimulator(Simulator *simulator);
    void synchronizeRenderer(RenderableObject *renderableObject);

public slots:
    void work();
};

class MySimulator : public Simulator
{
public:
    MySimulator();
    ~MySimulator();

protected:
    Worker *createWorker();
};

#endif // MYSIMULATOR_H
