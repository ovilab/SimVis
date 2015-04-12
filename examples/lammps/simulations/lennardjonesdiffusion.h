#ifndef LENNARDJONESDIFFUSION_H
#define LENNARDJONESDIFFUSION_H

#include "simulation.h"

class LennardJonesDiffusion : public Simulation
{
public:
    LennardJonesDiffusion();
    ~LennardJonesDiffusion();
    virtual void runLammpsScript(LAMMPS *lammps) override;

    // Simulation interface
protected:
    virtual void setColorEvaluator() override;
};

#endif // LENNARDJONESDIFFUSION_H
