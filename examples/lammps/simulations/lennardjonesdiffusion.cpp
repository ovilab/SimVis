#include "lennardjonesdiffusion.h"

LennardJonesDiffusion::LennardJonesDiffusion()
{
    m_simulationId = "lennardjonesdiffusion";
    m_inputScriptFile = ":/in.lennardjonesdiffusion";
    setColorEvaluator();
}

LennardJonesDiffusion::~LennardJonesDiffusion()
{

}



void LennardJonesDiffusion::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        colors.resize(lammps->atom->natoms);
        QColor color1(255.0, 0.0, 0.0);
        QColor color2(0.0, 255.0, 0.0);

        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            colors[i] = (lammps->atom->type[i] == 1) ? color1 : color2;
        }
    };
}
