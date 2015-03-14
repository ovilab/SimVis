#include "visualizer.h"
#include "renderable.h"
#include "billboards2d.h"
#include "simulator.h"

#include <QDebug>

Visualizer::Visualizer() :
    m_simulator(0)
{
}

Visualizer::~Visualizer()
{

}

void Visualizer::test()
{
}

QQuickFramebufferObject::Renderer *Visualizer::createRenderer() const
{
    qDebug() << "Renderer created";
    return new VisualizerRenderer();
}

Simulator *Visualizer::simulator() const
{
    return m_simulator;
}

void Visualizer::setSimulator(Simulator *arg)
{
    if (m_simulator == arg)
        return;

    m_simulator = arg;
    emit simulatorChanged(arg);
}

void VisualizerRenderer::render()
{
    qDebug() << "Render!";
    for(Renderable* renderable : m_renderables) {
        qDebug() << "Rendering " << renderable;
        QMatrix4x4 a;
        renderable->requestRender(a, a);
    }
}

void VisualizerRenderer::synchronize(QQuickFramebufferObject *fbo)
{
    qDebug() << "Synchronizing!";
    Visualizer* visualizer = static_cast<Visualizer*>(fbo);
    m_renderables = visualizer->findChildren<Renderable*>();
    for(Renderable* renderable : m_renderables) {
        qDebug() << "Rendering " << renderable;
        renderable->requestSynchronize();
    }
}
