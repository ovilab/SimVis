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
    void runCommands(const char *commands);
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
