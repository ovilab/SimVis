#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H

#include <SimVis/Simulator>
#include <SimVis/QuickWorker>
#include <QVector3D>

class MyWorker : public QuickWorker
{
    Q_OBJECT
public:
    MyWorker();

private:
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void work() override;
    virtual void synchronizeRenderer(Renderable *renderableObject) override;
    bool m_didSetScalarFieldEvaluator = false;
};

class MySimulator : public Simulator
{
    Q_OBJECT
protected:
    SimulatorWorker *createWorker();

private:
    friend class MyWorker;
};

#endif // MYSIMULATOR_H
