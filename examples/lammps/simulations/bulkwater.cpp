#include "bulkwater.h"
#include <iostream>
#include <QString>
#include <QDebug>
using namespace std;


BulkWater::BulkWater()
{
    setColorEvaluator();
    initialize(":/in.bulkwater");
}

void BulkWater::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        colors.resize(lammps->atom->natoms);
        QColor color1(255.0, 0.0, 0.0);
        QColor color2(255.0, 255.0, 255.0);

        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            if(lammps->atom->type[i] == 1) colors[i] = color1;
            else if(lammps->atom->type[i] == 2) colors[i] = color2;
        }
    };
}
