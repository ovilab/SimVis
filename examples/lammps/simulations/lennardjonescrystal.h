#ifndef LENNARDJONESCRYSTAL_H
#define LENNARDJONESCRYSTAL_H
#include "simulation.h"

class LennardJonesCrystal : public Simulation
{
public:
    LennardJonesCrystal();
    ~LennardJonesCrystal();

    // Simulation interface
protected:
    virtual void setColorEvaluator();
};

#endif // LENNARDJONESCRYSTAL_H
