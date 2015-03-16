#include "visualizer.h"
#include "renderable.h"
#include "billboards2d.h"
#include "simulator.h"
#include "camera.h"
#include <QDebug>
#include <QOpenGLFramebufferObjectFormat>

Visualizer::Visualizer()
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
}

Visualizer::~Visualizer()
{

}

VisualizerRenderer *Visualizer::createRenderer() const
{
    return new VisualizerRenderer();
}

Simulator *Visualizer::simulator() const
{
    return m_simulator;
}

Camera *Visualizer::camera()
{
    if(!m_camera) {
        m_camera = new Camera(static_cast<QObject*>(this));
    }
    return m_camera;
}

void Visualizer::setSimulator(Simulator *arg)
{
    if (m_simulator == arg) {
        return;
    }
    if(m_simulator) {
        disconnect(m_simulator, &Simulator::requestVisualizerSync, this, &Visualizer::synchronizeWorker);
    }
    m_simulator = arg;
    connect(m_simulator, &Simulator::requestVisualizerSync, this, &Visualizer::synchronizeWorker);
    emit simulatorChanged(arg);
}

void Visualizer::setCamera(Camera *arg)
{
    if (m_camera == arg)
        return;

    m_camera = arg;
    emit cameraChanged(arg);
}

void Visualizer::synchronizeWorker(SimulatorWorker *worker)
{
    QList<Renderable*> renderables = findChildren<Renderable*>();
    for(Renderable* renderable : renderables) {
        if(worker) {
            if(renderable->visible()) {
                worker->synchronizeRenderer(renderable);
            }
        }
    }
    update();
}

void Visualizer::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "Mouse moved while click: " << event->pos().x() << ", " << event->pos().y();
}

void Visualizer::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Mouse click: " << event->button();
}

void Visualizer::hoverEnterEvent(QHoverEvent *event)
{
    qDebug() << "Mouse enter";
}

void Visualizer::hoverMoveEvent(QHoverEvent *event)
{
    // qDebug() << "Mouse pos: " << event->pos().x() << ", " << event->pos().y();
}

void Visualizer::hoverLeaveEvent(QHoverEvent *event)
{
    qDebug() << "Mouse Leave";
}

void VisualizerRenderer::render()
{
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
    funcs.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    funcs.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for(Renderable* renderable : m_renderables) {
        if(renderable->visible()) {
            renderable->requestRender();
        }
    }
}

void VisualizerRenderer::synchronize(QQuickFramebufferObject *fbo)
{
    Visualizer* visualizer = static_cast<Visualizer*>(fbo);
    m_renderables = visualizer->findChildren<Renderable*>();
    m_camera = visualizer->camera();

    for(Renderable* renderable : m_renderables) {
        if(renderable->visible()) {
            if(!renderable->camera()) {
                renderable->setCamera(camera());
            }
            renderable->requestSynchronize();
        }
    }
}
Camera *VisualizerRenderer::camera() const
{
    return m_camera;
}

void VisualizerRenderer::setCamera(Camera *camera)
{
    m_camera = camera;
}


QOpenGLFramebufferObject *VisualizerRenderer::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}
