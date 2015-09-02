#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include <SimVis/Simulator>
#include <SimVis/QuickWorker>

#include <QVector3D>

class SimpleWorker : public QuickWorker
{
    Q_OBJECT
public:
    SimpleWorker();

private:
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void work() override;
    QVector<QVector3D> m_velocities;
};

class SimpleSimulator : public Simulator
{
    Q_OBJECT
protected:
    SimulatorWorker *createWorker();

private:
    friend class MyWorker;
};

#endif // MYSIMULATOR_H
