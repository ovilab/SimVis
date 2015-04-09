#include "mysimulator.h"
#include "library.h"
#include "input.h"
#include "atom.h"
#include <string>
#include <sstream>
#include <SimVis/Spheres>
using std::string;

MySimulator::MySimulator()
{

}

MySimulator::~MySimulator()
{

}

MyWorker::MyWorker() {
    lammps_open_no_mpi(0, 0, (void**)&lammps);

    string cmd = "units		lj\n"
                 "atom_style	atomic\n"
                 "atom_modify	map hash\n"
                 "\n"
                 "lattice		fcc 0.8442\n"
                 "region		box block 0 10 0 10 0 10\n"
                 "create_box	1 box\n"
                 "create_atoms	1 box\n"
                 "mass		1 1.0\n"
                 "\n"
                 "velocity	all create 1.44 87287 loop geom\n"
                 "\n"
                 "pair_style	lj/cut 2.5\n"
                 "pair_coeff	1 1 1.0 1.0 2.5\n"
                 "\n"
                 "neighbor	0.3 bin\n"
                 "neigh_modify	delay 0 every 20 check no\n"
                 "\n"
                 "fix		1 all nve\n";
    runCommands(cmd.c_str());
    lammps->input->one("run 1");
}

void MyWorker::runCommands(const char *commands) {
    std::stringstream ss(commands);
    std::string to;

    if (commands != NULL)
    {
        while(std::getline(ss,to,'\n')){
            lammps->input->one(to.c_str());
        }
    }
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{

}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    Spheres* billboards = qobject_cast<Spheres*>(renderableObject);
    if(billboards) {
        billboards->positions().resize(lammps->atom->natoms);
        QVector<QVector3D> &positions = billboards->positions();

        for(unsigned int i=0; i<lammps->atom->natoms; i++) {
            positions[i][0] = lammps->atom->x[i][0];
            positions[i][1] = lammps->atom->x[i][1];
            positions[i][2] = lammps->atom->x[i][2];
        }
        return;
    }
}

void MyWorker::work()
{
    lammps->input->one("run 1");
}

MyWorker *MySimulator::createWorker()
{
    return new MyWorker();
}
