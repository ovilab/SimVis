#include "renderable.h"

Renderable::Renderable(QObject *parent) :
    QObject(parent),
    m_renderer(0)
{

}

Renderable::~Renderable()
{

}

void Renderable::requestRender()
{
    if(!m_renderer) {
        m_renderer = createRenderer();
    }
    m_renderer->prepareAndRender();
}

void Renderable::requestSynchronize()
{
    if(!m_renderer) {
        m_renderer = createRenderer();
    }
    m_renderer->synchronize(this);
}

void Renderable::setVisible(bool arg)
{
    if (m_visible == arg)
        return;

    m_visible = arg;
    emit visibleChanged(arg);
}

void RenderableRenderer::generateVBOs()
{
    if(m_numberOfVBOs>0) {
        m_vboIds.resize(m_numberOfVBOs);
        glFunctions()->glGenBuffers(m_numberOfVBOs, &m_vboIds.front());
    }
}

void RenderableRenderer::prepareAndRender()
{
    if(!m_program.isLinked()) {
        beforeLinkProgram();
        m_program.link();
    }
    m_program.bind();
    render();
    m_program.release();
}

QOpenGLFunctions* RenderableRenderer::glFunctions() {
    if(!m_funcs) {
        m_funcs = new QOpenGLFunctions(QOpenGLContext::currentContext());
    }
    return m_funcs;
}

QOpenGLShaderProgram &RenderableRenderer::program()
{
    return m_program;
}
