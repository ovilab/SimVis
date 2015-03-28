#include "renderable.h"
#include "camera.h"
#include <QFile>

Renderable::Renderable(QQuickItem *parent) :
    QQuickItem(parent),
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
    m_renderer->m_ambient = m_ambient;
    m_renderer->m_diffuse = m_diffuse;
    m_renderer->m_specular = m_specular;
    m_renderer->m_diffuseIntensity = m_diffuseIntensity;
    m_renderer->m_ambientIntensity = m_ambientIntensity;
    m_renderer->m_attenuation = m_attenuation;
    m_renderer->m_shininess = m_shininess;
    m_renderer->m_lightPosition = m_lightPosition;
    m_renderer->m_specularIntensity = m_specularIntensity;
    m_renderer->copyShaderEffects(this);

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

QColor Renderable::ambient() const
{
    return m_ambient;
}

QColor Renderable::diffuse() const
{
    return m_diffuse;
}

QVector3D Renderable::lightPosition() const
{
    return m_lightPosition;
}

float Renderable::diffuseIntensity() const
{
    return m_diffuseIntensity;
}

float Renderable::ambientIntensity() const
{
    return m_ambientIntensity;
}

float Renderable::shininess() const
{
    return m_shininess;
}

QColor Renderable::specular() const
{
    return m_specular;
}

float Renderable::attenuation() const
{
    return m_attenuation;
}

float Renderable::specularIntensity() const
{
    return m_specularIntensity;
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

void Renderable::setAmbient(QColor arg)
{
    if (m_ambient == arg)
        return;

    m_ambient = arg;
    emit ambientChanged(arg);
}

void Renderable::setDiffuse(QColor arg)
{
    if (m_diffuse == arg)
        return;

    m_diffuse = arg;
    emit diffuseChanged(arg);
}

void Renderable::setLightPosition(QVector3D arg)
{
    if (m_lightPosition == arg)
        return;

    m_lightPosition = arg;
    emit lightPositionChanged(arg);
}

void Renderable::setDiffuseIntensity(float arg)
{
    if (m_diffuseIntensity == arg)
        return;

    m_diffuseIntensity = arg;
    emit diffuseIntensityChanged(arg);
}

void Renderable::setAmbientIntensity(float arg)
{
    if (m_ambientIntensity == arg)
        return;

    m_ambientIntensity = arg;
    emit ambientIntensityChanged(arg);
}

void Renderable::setShininess(float arg)
{
    if (m_shininess == arg)
        return;

    m_shininess = arg;
    emit shininessChanged(arg);
}

void Renderable::setSpecular(QColor arg)
{
    if (m_specular == arg)
        return;

    m_specular = arg;
    emit specularChanged(arg);
}

void Renderable::setAttenuation(float arg)
{
    if (m_attenuation == arg)
        return;

    m_attenuation = arg;
    emit attenuationChanged(arg);
}

void Renderable::setSpecularIntensity(float arg)
{
    if (m_specularIntensity == arg)
        return;

    m_specularIntensity = arg;
    emit specularIntensityChanged(arg);
}

RenderableRenderer::RenderableRenderer()
{
    m_elapsedTime.start();
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
    // If any of the shader effects is marked dirty, we need to recompile the shaders
    for(ShaderEffect *shaderEffect : m_shaderEffects) {
        if(shaderEffect->shadersDirty()) {
            m_shadersDirty = true;
            shaderEffect->setShadersDirty(false);
        }
    }

    if(m_shadersDirty) {
        m_fragmentShaderBase.clear();
        m_vertexShaderBase.clear();
        addShaderCodeToBase(QOpenGLShader::Fragment, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/default.glsl"));
        addShaderCodeToBase(QOpenGLShader::Vertex, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/default.glsl"));

        for(ShaderEffect *shaderEffect : m_shaderEffects) {
            if(shaderEffect->enabled()) {
                // Defines must come before library
                addShaderCodeToBase(QOpenGLShader::Fragment, shaderEffect->fragmentShaderDefines());
                addShaderCodeToBase(QOpenGLShader::Vertex, shaderEffect->vertexShaderDefines());
                addShaderCodeToBase(QOpenGLShader::Fragment, shaderEffect->fragmentShaderLibrary());
                addShaderCodeToBase(QOpenGLShader::Vertex, shaderEffect->vertexShaderLibrary());
            }
        }

        beforeLinkProgram();
        m_program.link();
        m_shadersDirty = false;
    }

    m_program.bind();
    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    m_program.setUniformValue("cp_modelViewProjectionMatrix", modelViewProjectionMatrix);
    m_program.setUniformValue("cp_modelViewMatrix", m_modelViewMatrix);
    m_program.setUniformValue("cp_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("cp_viewVector", m_viewVector);
    m_program.setUniformValue("cp_cameraPosition", m_cameraPosition);
    m_program.setUniformValue("cp_time", float(m_elapsedTime.elapsed()*1e-3));
    for(ShaderEffect *shaderEffect : m_shaderEffects) {
        if(shaderEffect->enabled()) {
            shaderEffect->setUniformValues(m_program);
        }
    }

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

void RenderableRenderer::copyShaderEffects(Renderable *renderable)
{
    // Remove the copies we have in renderer
    for(ShaderEffect *shaderEffect : m_shaderEffects) {
        delete shaderEffect;
    }
    m_shaderEffects.clear();

    QList<ShaderEffect*> shaderEffects = renderable->findChildren<ShaderEffect*>();
    for(ShaderEffect *shaderEffect : shaderEffects) {
        ShaderEffect *shaderEffectClone = shaderEffect->clone();
        m_shaderEffects.push_back(shaderEffectClone);
        shaderEffect->setShadersDirty(false);
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
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin2) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/perlin2.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin3) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/perlin3.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Perlin4) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/perlin4.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex2) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/simplex2.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex3) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/simplex3.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Simplex4) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/simplex4.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::ColorEffects) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/coloreffects.glsl"));
    if(shader == CompPhys::AllShaders || shader == CompPhys::Tools) addShaderCodeToBase(type, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/tools.glsl"));
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
