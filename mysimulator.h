#pragma once
#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "simulator.h"
#include <QDebug>
#include <QVector2D>

class MyWorker : public SimulatorWorker {
    Q_OBJECT
public:
    MyWorker(QObject* parent = 0);
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
    Q_OBJECT
public:
    MySimulator(QObject* parent = 0);
    ~MySimulator();

protected:
    SimulatorWorker *createWorker();
};

#endif // MYSIMULATOR_H
