#include "lennardjonescrystal.h"

LennardJonesCrystal::LennardJonesCrystal()
{
    m_simulationId = "lennardjonescrystal";
    m_inputScriptFile = ":/in.lennardjonescrystal";
    setColorEvaluator();
}

LennardJonesCrystal::~LennardJonesCrystal()
{

}



void LennardJonesCrystal::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        colors.resize(lammps->atom->natoms);

        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            colors[i] = QColor(110, 212, 219);
        }
    };
}
