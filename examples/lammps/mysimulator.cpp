#include "mysimulator.h"
#include "library.h"
#include "atom.h"
#include "domain.h"
#include "update.h"
#include <string>
#include <sstream>
#include <SimVis/Spheres>
#include <QUrl>
#include <QString>
#include <QQmlFile>

using std::string;

MySimulator::MySimulator()
{

}

MySimulator::~MySimulator()
{

}

MyWorker::MyWorker() {
    lammps_open_no_mpi(0, 0, (void**)&lammps);

    QFile file(":/in.lj");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file: " << file.fileName();
    }
    QString content = file.readAll();
    runCommands(content.toStdString().c_str());
    // runCommand("run 1");
}

void MyWorker::runCommands(const char *commands) {
    std::stringstream ss(commands);
    std::string to;

    if (commands != NULL)
    {
        while(std::getline(ss,to,'\n')){
            runCommand(to.c_str());
        }
    }
}

void MyWorker::runCommand(const char *command)
{
    // qDebug() << command;
    lammps_command((void*)lammps, (char*) command);
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{

}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    Spheres* spheres = qobject_cast<Spheres*>(renderableObject);
    if(spheres) {
        spheres->positions().resize(lammps->atom->natoms);
        QVector<QVector3D> &positions = spheres->positions();
        double position[3];
        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            position[0] = lammps->atom->x[i][0];
            position[1] = lammps->atom->x[i][1];
            position[2] = lammps->atom->x[i][2];
            lammps->domain->remap(position);

            positions[i][0] = position[0] - lammps->domain->prd_half[0];
            positions[i][1] = position[1] - lammps->domain->prd_half[0];
            positions[i][2] = position[2] - lammps->domain->prd_half[0];
        }
        return;
    }
}

void MyWorker::work()
{
//    if( (m_timestepsSinceLastPreRun % 100) == 0) {
//        for(QString &command : m_queuedCommands) {
//            lammps_command((void*)lammps, (char*)command.toStdString().c_str());
//        }

//        m_lastPreRun = lammps->update->ntimestep;
//        QString cmd = QString("run 1 start %1 stop %2").arg(m_lastPreRun).arg(m_lastPreRun+100);

//        runCommand(cmd.toStdString().c_str());
//    } else {
//        QString cmd = QString("run 1 pre no post no start %1 stop %2").arg(m_lastPreRun).arg(m_lastPreRun+100);
//        runCommand(cmd.toStdString().c_str());
//    }
    runCommand("run 1 pre no post no");
}

MyWorker *MySimulator::createWorker()
{
    return new MyWorker();
}
