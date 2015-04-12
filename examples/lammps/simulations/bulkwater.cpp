#include "bulkwater.h"
#include <iostream>
using namespace std;


BulkWater::BulkWater()
{
    m_simulationId = "bulkwater";
    m_inputScriptFile = ":/in.bulkwater";
    setColorEvaluator();
}

BulkWater::~BulkWater()
{

}



void BulkWater::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        colors.resize(lammps->atom->natoms);
        QColor color1(255.0, 0.0, 0.0);
        QColor color2(255.0, 255.0, 255.0);

        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            colors[i] = (lammps->atom->type[i] == 1) ? color1 : color2;
        }
    };
}

void BulkWater::runLammpsScript(LAMMPS *lammps)
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
                cout << "Found read_data: " << to << endl;
            }
            cout << "Command: " << to << endl;
            runCommand(lammps, to.c_str());
        }
    }
}
