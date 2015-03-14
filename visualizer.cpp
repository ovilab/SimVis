#include "visualizer.h"
#include "renderable.h"
#include "billboards2d.h"
#include "simulator.h"

#include <QDebug>
#include <QOpenGLFramebufferObjectFormat>

Visualizer::Visualizer()
{
}

Visualizer::~Visualizer()
{

}

void Visualizer::test()
{
    QList<Renderable*> renderables = findChildren<Renderable*>();
    for(Renderable* renderable : renderables) {
        if(m_simulator->m_worker) {
            m_simulator->m_worker->synchronizeRenderer(renderable);
        }
    }
}

VisualizerRenderer *Visualizer::createRenderer() const
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
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    funcs.glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
    funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    qDebug() << "Rendering!";
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
        qDebug() << "Synchronizing " << renderable;
        renderable->requestSynchronize();
    }
}

QOpenGLFramebufferObject *VisualizerRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}
