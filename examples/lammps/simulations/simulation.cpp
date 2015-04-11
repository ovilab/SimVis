#include "simulation.h"

Simulation::Simulation()
{

}

Simulation::~Simulation()
{

}

function<void (QVector<QColor> &colors, LAMMPS *lammps)> Simulation::colorEvaluator()
{
    return m_colorEvaluator;
}

QString Simulation::simulationId()
{
    return m_simulationId;
}

QString Simulation::inputScriptFile()
{
    return m_inputScriptFile;
}

