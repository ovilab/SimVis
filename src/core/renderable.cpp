#include "renderable.h"
#include "camera.h"
#include <QFile>

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
    m_renderer->m_modelViewMatrix = m_camera->matrix();
    m_renderer->m_projectionMatrix = m_camera->projectionMatrix();
    m_renderer->m_viewVector = m_camera->viewVector();
    m_renderer->m_cameraPosition = m_camera->position();
    m_renderer->synchronize(this);
}

bool Renderable::visible() const
{
    return m_visible;
}

Camera *Renderable::camera() const
{
    return m_camera;
}

void Renderable::setVisible(bool arg)
{
    if (m_visible == arg)
        return;

    m_visible = arg;
    emit visibleChanged(arg);
}

void Renderable::setCamera(Camera *arg)
{
    if (m_camera == arg)
        return;

    m_camera = arg;
    emit cameraChanged(arg);
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
    if(m_shadersDirty) {
        // TODO: Only build the shader bases once?
        m_fragmentShaderBase.clear();
        m_vertexShaderBase.clear();
        beforeLinkProgram();
        m_program.link();
        m_shadersDirty = false;
    }
    m_program.bind();
    render();
    m_program.release();
}

void RenderableRenderer::removeShader(QOpenGLShader::ShaderType type) {
    // Remove a shader of this type if it exists
    for(QOpenGLShader *shader : m_program.shaders()) {
        if(shader->shaderType() == type) {
            m_program.removeShader(shader);
            return;
        }
    }
}

QString RenderableRenderer::contentFromFile(QString fileName) {
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open " << f.fileName() << ". Aborting!";
        exit(1);
    }
    if(!f.isOpen()) {
        qDebug() << "Could not open " << f.fileName() << ". Aborting!";
        exit(1);
    }
    QTextStream stream(&f);
    QString content = "\n";
    content.append(stream.readAll());
    content.append("\n");
    f.close();

    return content;
}

void RenderableRenderer::setShaderFromSourceCode(QOpenGLShader::ShaderType type, QString shaderCode) {
    QString fullShaderCode;
    if(type == QOpenGLShader::Vertex) {
        fullShaderCode = m_vertexShaderBase;
    } else if(type == QOpenGLShader::Fragment) {
        fullShaderCode = m_fragmentShaderBase;
    } else {
        qDebug() << "Shaders of this type aren't supported yet.";
        return;
    }

    fullShaderCode.append(shaderCode);
    removeShader(type);
    m_program.addShaderFromSourceCode(type, fullShaderCode);
}

void RenderableRenderer::setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName) {
    QString shaderCode = contentFromFile(fileName);
    setShaderFromSourceCode(type, shaderCode);
}

void RenderableRenderer::addShaderCodeToBase(QOpenGLShader::ShaderType type, QString shaderCode) {
    if(type == QOpenGLShader::Fragment) m_fragmentShaderBase.append(shaderCode);
    else if(type == QOpenGLShader::Vertex) m_vertexShaderBase.append(shaderCode);
    else qDebug() << "Shaders of this type aren't supported yet.";
}

void RenderableRenderer::addShaderLibrary(QOpenGLShader::ShaderType type, CompPhys::Shader shader)
{
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin2) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/perlin2.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin3) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/perlin3.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin4) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/perlin4.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex2) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/simplex2.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex3) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/simplex3.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex4) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/simplex4.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::ColorEffects) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/renderables/shadereffects/coloreffects.glsl"));
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
