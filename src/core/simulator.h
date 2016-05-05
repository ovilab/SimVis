#pragma once
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QMutex>
#include <QQuickItem>

#include <Qt3DRender/QBuffer>
#include <Qt3DCore/QEntity>

class Renderable;
class Simulator;

class SimulatorWorker : public QObject {
    Q_OBJECT

protected:
    virtual void work() = 0;

private:
    virtual void synchronizeSimulator(Simulator *simulator) = 0;
    Q_INVOKABLE void workAndUnlock(Simulator *simulator);
signals:
    void workDone();

private:
    friend class Simulator;
};

class Simulator : public Qt3DCore::QNode
{
    Q_OBJECT
public:
    explicit Simulator(QNode *parent = 0);
    virtual ~Simulator();

protected:
    virtual SimulatorWorker *createWorker() = 0;

public slots:
    void step();

private:
    SimulatorWorker *m_worker = 0;
    QThread m_workerThread;
    QTimer  m_timer;
    QMutex m_workerMutex;

    friend class SimulatorWorker;
};

#endif // SIMULATOR_H
