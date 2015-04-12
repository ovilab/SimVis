#include "gashydrates.h"

GasHydrates::GasHydrates()
{
    m_simulationId = "gashydrates";
    m_inputScriptFile = ":/in.gashydrates";
    setColorEvaluator();
}

GasHydrates::~GasHydrates()
{

}



void GasHydrates::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        colors.resize(lammps->atom->natoms);
        QColor color1(255.0, 0.0, 0.0);
        QColor color2(255.0, 255.0, 255.0);
        QColor color3(110, 212, 219);
        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            if(lammps->atom->type[i] == 1) colors[i] = color1;
            else if(lammps->atom->type[i] == 2) colors[i] = color2;
            else if(lammps->atom->type[i] == 3) colors[i] = color3;
        }
    };
}
