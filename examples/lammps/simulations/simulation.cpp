#include "simulation.h"
#include <QFile>
#include <QDebug>
#include <QStandardPaths>

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

QString Simulation::copyDataFileToReadablePath(QString filename)
{
    QFile inFile(":/"+filename);
    if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error: could not open file "+inFile.fileName();
        return "";
    }

    QString content = inFile.readAll();
    inFile.close();

    QString documentsLocation = QStandardPaths::locate(QStandardPaths::TempLocation, QString(), QStandardPaths::LocateDirectory);
    QString newFilename=documentsLocation+filename;
    QFile outFile(newFilename);
    if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Error: could not open file "+outFile.fileName();
        return "";
    }

    outFile.write(content.toStdString().c_str());
    outFile.close();
    return newFilename;
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

void Simulation::runLammpsScript(LAMMPS *lammps)
{
    QString lammpsScript = readFile(m_inputScriptFile);
    std::stringstream ss(lammpsScript.toStdString());
    std::string to;

    if (!lammpsScript.isEmpty())
    {
        while(std::getline(ss,to,'\n')){
            std::stringstream commandLine(to);
            std::string command;
            commandLine >> command;
            if(command.compare("read_data") == 0) {
                // If read_data command is found, we need to copy the file to a readable place for LAMMPS
                std::string dataFile;
                commandLine >> dataFile;
                QString newFilePath = copyDataFileToReadablePath(QString::fromStdString(dataFile));
                QString command = "read_data "+newFilePath;
                runCommand(lammps, command.toStdString().c_str());
            } else {
                runCommand(lammps, to.c_str());
            }
        }
    }
}

