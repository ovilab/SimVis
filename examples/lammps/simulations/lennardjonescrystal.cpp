#include "lennardjonescrystal.h"

LennardJonesCrystal::LennardJonesCrystal()
{
    setColorEvaluator();
    initialize(":/in.lennardjonescrystal");
}

void LennardJonesCrystal::setColorEvaluator()
{
    m_colorEvaluator = [](QVector<QColor> &colors, LAMMPS *lammps) {
        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            colors[i] = QColor(110, 212, 219);
        }
    };
}
