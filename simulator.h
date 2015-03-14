#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDebug>
class Renderable;
class Simulator;

class Worker : public QObject {
    Q_OBJECT
public:
    virtual void synchronizeSimulator(Simulator *simulator) = 0;
    virtual void synchronizeRenderer(Renderable *renderableObject) = 0;
public slots:
    virtual void work() = 0;
};

class Simulator : public QObject
{
    Q_OBJECT
public:


    explicit Simulator(QObject *parent = 0);
    ~Simulator();
protected:
    virtual Worker *createWorker() = 0;

signals:
    void requestRendererSync(Worker *worker);
    void requestWork();

public slots:
    void step();

private:
    Worker *m_worker = 0;
    QThread m_workerThread;
    QTimer  m_timer;

    friend class Visualizer;
};

#endif // SIMULATOR_H
