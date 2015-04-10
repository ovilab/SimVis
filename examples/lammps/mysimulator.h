#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include "mpi.h"
#include "lammps.h"

using namespace LAMMPS_NS;

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();
private:
    LAMMPS *lammps = 0;
    // SimulatorWorker interface
private:
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void synchronizeRenderer(Renderable *renderableObject) override;
    virtual void work() override;
    unsigned int m_timestepsSinceLastPreRun = 0;
    unsigned int m_lastPreRun = 0;
    QVector<QString> m_queuedCommands;
    void runCommands(const char *commands);
    void runCommand(const char *command);
};

class MySimulator : public Simulator
{
public:
    MySimulator();
    ~MySimulator();

    // Simulator interface
protected:
    virtual MyWorker *createWorker() override;
};

#endif // MYSIMULATOR_H
