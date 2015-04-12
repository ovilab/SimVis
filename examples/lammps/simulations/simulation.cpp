#include "simulation.h"
#include <QFile>
#include <QDebug>

QString Simulation::readFile(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file: " << file.fileName();
        return "";
    }

    return file.readAll();
}

void Simulation::runCommand(LAMMPS *lammps, const char *command)
{
    if(lammps == 0) {
        qDebug() << "Warning, trying to run a LAMMPS command with no LAMMPS object. You need to load a simulation first.";
        qDebug() << "Command: " << command;
        return;
    }

    lammps_command((void*)lammps, (char*) command);
}

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

