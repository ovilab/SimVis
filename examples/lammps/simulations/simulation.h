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

    function<void(QVector<QColor> &colors, LAMMPS *lammps)> m_colorEvaluator;
    QString m_simulationId;
    QString m_inputScriptFile;
    QMap<QString, QString> m_additionalFiles;
    virtual void setColorEvaluator() = 0;
public:
    Simulation();
    ~Simulation();
    function<void(QVector<QColor> &colors, LAMMPS *lammps)> colorEvaluator();
    QString simulationId();
    QString inputScriptFile();
    virtual void runLammpsScript(LAMMPS *lammps) = 0;
};

#endif // SIMULATION_H
