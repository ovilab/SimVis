#ifndef GASHYDRATES_H
#define GASHYDRATES_H

#include "simulation.h"
class GasHydrates : public Simulation
{
public:
    GasHydrates();
    ~GasHydrates();

    // Simulation interface
protected:
    virtual void setColorEvaluator();
};

#endif // GASHYDRATES_H
