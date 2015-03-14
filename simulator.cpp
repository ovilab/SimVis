#include "simulator.h"

Simulator::Simulator(QObject *parent) : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &Simulator::step);
    m_timer.start(1);
}

Simulator::~Simulator()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void Simulator::step()
{
    qDebug() << "Stepping simulator!";
    if(!m_worker) {
        m_worker = createWorker();
        m_worker->moveToThread(&m_workerThread);
        connect(this, &Simulator::requestWork, m_worker, &SimulatorWorker::work);
        m_workerThread.start();
    }
    qDebug() << "Syncrhonizing simulator!";
    m_worker->synchronizeSimulator(this);
    qDebug() << "Requesting renderer sync!";
    emit requestRendererSync(m_worker);
    qDebug() << "Requesting work!";
    emit requestWork();
}

