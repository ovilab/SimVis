#ifndef SIMULATION_H
#define SIMULATION_H
#include <functional>
#include <QVector>
#include <QColor>
#include "lammps/mpi.h"
#include "lammps/lammps.h"
#include "lammps/atom.h"
using std::function;
using namespace LAMMPS_NS;

class Simulation
{
protected:
    function<void(QVector<QColor> &colors, LAMMPS *lammps)> m_colorEvaluator;
    QString m_simulationId;
    QString m_inputScriptFile;
    virtual void setColorEvaluator() = 0;
public:
    Simulation();
    ~Simulation();
    function<void(QVector<QColor> &colors, LAMMPS *lammps)> colorEvaluator();
    QString simulationId();
    QString inputScriptFile();
};

#endif // SIMULATION_H
