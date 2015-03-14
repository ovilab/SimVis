#include "simulator.h"

Simulator::Simulator(QObject *parent) : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &Simulator::step);
    m_timer.start(1000);
}

Simulator::~Simulator()
{

}

void Simulator::step()
{
    if(!m_worker) {
        m_worker = createWorker();
        m_worker->moveToThread(&m_workerThread);
        connect(this, &Simulator::requestWork, m_worker, &SimulatorWorker::work);
        m_workerThread.start();
    }
    m_worker->synchronizeSimulator(this);
    emit requestRendererSync(m_worker);
    emit requestWork();
}

