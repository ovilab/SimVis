#include "bulkwater.h"
#include <iostream>
#include <QString>
#include <QDebug>
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
