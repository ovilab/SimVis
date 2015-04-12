#ifndef SIMULATION_H
#define SIMULATION_H
#include <functional>
#include <sstream>
#include <string>
#include <QVector>
#include <QColor>
#include <QMap>
#include "lammps/mpi.h"
#include "lammps/lammps.h"
#include "lammps/library.h"
#include "lammps/atom.h"
using std::function;
using namespace LAMMPS_NS;

class Simulation
{
protected:
    QString readFile(QString filename);
    void runCommand(LAMMPS *lammps, const char *command);
    QString copyDataFileToReadablePath(QString filename);

    function<void(QVector<QColor> &colors, LAMMPS *lammps)> m_colorEvaluator;
    bool m_isInitialized = false;
    QString m_inputScriptFile;
    virtual void setColorEvaluator();
    void initialize(QString inputScriptFile);
public:
    Simulation();
    ~Simulation();
    function<void(QVector<QColor> &colors, LAMMPS *lammps)> colorEvaluator();
    QString simulationId();
    QString inputScriptFile();
    void runLammpsScript(LAMMPS *lammps);
};

#endif // SIMULATION_H
