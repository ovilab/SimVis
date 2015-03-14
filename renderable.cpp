#include "renderable.h"

Renderable::Renderable(QObject *parent) :
    QObject(parent),
    m_renderer(0)
{

}

Renderable::~Renderable()
{

}

void Renderable::afterSynchronize() {

}

void Renderable::requestRender()
{
    if(!m_renderer) {
        m_renderer = createRenderer();
    }
    m_renderer->render();
}

void Renderable::requestSynchronize()
{
    if(!m_renderer) {
        m_renderer = createRenderer();
    }
    m_renderer->synchronize(this);
}

void RenderableRenderer::generateVBOs()
{
    if(m_numberOfVBOs>0) {
        m_vboIds.resize(m_numberOfVBOs);
        glFunctions()->glGenBuffers(m_numberOfVBOs, &m_vboIds.front());
    }
}

QOpenGLFunctions* RenderableRenderer::glFunctions() {
    if(!m_funcs) {
        m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
    }
    return m_funcs;
}
