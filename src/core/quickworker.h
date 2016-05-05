#ifndef QUICKWORKER_H
#define QUICKWORKER_H

#include "simulator.h"

#include <QVector>
#include <QVector3D>

class QuickWorker : public SimulatorWorker
{
    Q_OBJECT
private:
    friend class Simulator;
protected:
    QVector<QVector3D> m_positions;
public:
    QuickWorker();
    ~QuickWorker();
};

#endif // QUICKWORKER_H
