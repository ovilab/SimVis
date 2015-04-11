#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <functional>
#include <QMap>
#include "simulations/simulations.h"

#include "lammps/mpi.h"
#include "lammps/lammps.h"
using std::function;
using namespace LAMMPS_NS;

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();
private:
    LAMMPS *lammps = 0;

    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void synchronizeRenderer(Renderable *renderableObject) override;
    virtual void work() override;
    function<void(QVector<QColor> &colors, LAMMPS *lammps)> m_colorPicker = 0;
    unsigned int m_timestepsSinceLastPreRun = 0;
    unsigned int m_lastPreRun = 0;
    unsigned int m_simulationSpeed = 1;
    bool m_discoMode = false;
    QMap<QString, Simulation*> m_simulations;
    QVector<QString> m_queuedCommands;
    void runCommands(const char *commands);
    void runCommand(const char *command);
    void loadSimulation(QString inputScript);
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(bool discoMode READ discoMode WRITE setDiscoMode NOTIFY discoModeChanged)
    Q_PROPERTY(int simulationSpeed READ simulationSpeed WRITE setSimulationSpeed NOTIFY simulationSpeedChanged)

public:
    MySimulator();
    ~MySimulator();

    // Simulator interface
    bool discoMode() const;
    int simulationSpeed() const;

public slots:
    void setDiscoMode(bool arg);
    void loadSimulation(QString simulation);
    void setSimulationSpeed(int arg);

signals:
    void discoModeChanged(bool arg);
    void simulationSpeedChanged(int arg);

protected:
    bool m_discoMode = false;
    bool m_willLoadSimulation = false;
    int m_simulationSpeed = 1;

    virtual MyWorker *createWorker() override;
};

#endif // MYSIMULATOR_H
