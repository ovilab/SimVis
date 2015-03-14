#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "simulator.h"
#include <QDebug>

class MyWorker : public SimulatorWorker {
public:
    MyWorker();
    void synchronizeSimulator(Simulator *simulator);
    void synchronizeRenderer(Renderable *renderableObject);

public slots:
    void work();

private:
    std::vector<QVector2D> m_positions;
    std::vector<QVector2D> m_velocities;
};

class MySimulator : public Simulator
{
public:
    MySimulator();
    ~MySimulator();

protected:
    SimulatorWorker *createWorker();
};

#endif // MYSIMULATOR_H
