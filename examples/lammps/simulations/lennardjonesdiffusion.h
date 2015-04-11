#ifndef LENNARDJONESDIFFUSION_H
#define LENNARDJONESDIFFUSION_H

#include "simulation.h"

class LennardJonesDiffusion : public Simulation
{
public:
    LennardJonesDiffusion();
    ~LennardJonesDiffusion();

    // Simulation interface
protected:
    virtual void setColorEvaluator();
};

#endif // LENNARDJONESDIFFUSION_H
