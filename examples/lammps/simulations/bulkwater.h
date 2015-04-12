#ifndef BULKWATER_H
#define BULKWATER_H
#include "simulation.h"

class BulkWater : public Simulation
{
public:
    BulkWater();
    ~BulkWater();

    // Simulation interface
protected:
    virtual void setColorEvaluator();

public:
    virtual void runLammpsScript(LAMMPS *lammps);
};

#endif // BULKWATER_H
