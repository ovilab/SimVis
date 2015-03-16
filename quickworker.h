#ifndef QUICKWORKER_H
#define QUICKWORKER_H
#include <QVector>
#include "simulator.h"

class QuickWorker : public SimulatorWorker
{
    Q_OBJECT
private:
    friend class Simulator;
protected:
    QVector<QVector2D> m_positions;
public:
    QuickWorker();
    ~QuickWorker();

    // SimulatorWorker interface
private:
    void synchronizeRenderer(Renderable *renderableObject);
};

#endif // QUICKWORKER_H
