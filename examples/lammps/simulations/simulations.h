#include "simulation.h"
#include "lennardjonescrystal.h"
#include "lennardjonesdiffusion.h"
#include "bulkwater.h"
#include "gashydrates.h"

class Crack : public Simulation
{
public:
    Crack() { initialize(":/in.crack"); }
};

class FlowPoiseuille : public Simulation
{
public:
    FlowPoiseuille() { initialize(":/in.flow.poiseuille"); }
};

class FlowCouette : public Simulation
{
public:
    FlowCouette() { initialize(":/in.flow.couette"); }
};

class Friction : public Simulation
{
public:
    Friction() { initialize(":/in.friction"); }
};

class Obstacle : public Simulation
{
public:
    Obstacle() { initialize(":/in.obstacle"); }
};
