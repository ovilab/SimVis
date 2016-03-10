#include "renderable.h"
#include "camera.h"
#include "../shadereffects/light.h"
#include <QFile>
#include <QOpenGLFunctions_4_1_Core>

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
    setGeometryShaderSupported(m_renderer->isGeometryShadersSupported());

    m_renderer->m_modelViewMatrix = m_camera->matrix();
    m_renderer->m_projectionMatrix = m_camera->projectionMatrix();
    m_renderer->m_viewVector = m_camera->viewVector().normalized();
    m_renderer->m_viewCenter = m_camera->viewCenter();
    m_renderer->m_upVector = m_camera->upVector().normalized();
    m_renderer->m_rightVector = QVector3D::crossProduct(m_renderer->m_viewVector, m_renderer->m_upVector);
    m_renderer->m_cameraPosition = m_camera->position();
    m_renderer->m_modelViewMatrixInverse = m_renderer->m_modelViewMatrix.inverted();
    m_renderer->m_projectionMatrixInverse = m_renderer->m_projectionMatrix.inverted();
    m_renderer->copyShaderEffects(this);

    if(m_hasDirtyShaders) {
        m_renderer->m_shadersDirty = true;
        if(!vertexShader() || !fragmentShader()) {
            qWarning() << "Renderable::requestSynchronize(): Missing shaders.";
            return;
        }
        m_renderer->m_vertexShaderSource = vertexShader()->finalShader();
        if(geometryShader()) {
            m_renderer->m_geometryShaderSource = geometryShader()->finalShader();
        }
        m_renderer->m_fragmentShaderSource = fragmentShader()->finalShader();
        m_hasDirtyShaders = false;
    }

    m_renderer->m_uniforms = fragmentShader()->uniforms();

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

bool Renderable::isGeometryShaderSupported() const
{
    return m_isGeometryShaderSupported;
}

ShaderBuilder *Renderable::fragmentShader() const
{
    return m_fragmentShader;
}

ShaderBuilder *Renderable::geometryShader() const
{
    return m_geometryShader;
}

ShaderBuilder *Renderable::vertexShader() const
{
    return m_vertexShader;
}

void Renderable::setGeometryShaderSupported (bool arg)
{
    if (m_isGeometryShaderSupported  == arg)
        return;

    m_isGeometryShaderSupported  = arg;
    emit isGeometryShaderSupportedChanged(arg);
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

void Renderable::triggerDirtyShaders() {
    setDirtyShaders(true);
}

void Renderable::triggerDirtyData()
{
    setDirtyData(true);
}

bool Renderable::hasDirtyData() const
{
    return m_hasDirtyData;
}

bool Renderable::hasDirtyShaders() const
{
    return m_hasDirtyShaders;
}

void Renderable::setFragmentShader(ShaderBuilder *fragmentShader)
{
    if (m_fragmentShader == fragmentShader)
        return;
    if(m_fragmentShader) {
        disconnect(m_fragmentShader, 0, this, 0);
    }
    m_fragmentShader = fragmentShader;
    connect(m_fragmentShader, &ShaderBuilder::finalShaderChanged, this, &Renderable::triggerDirtyShaders);
    connect(m_fragmentShader, &ShaderBuilder::uniformsChanged, this, &Renderable::triggerDirtyData);
    triggerDirtyShaders();
    triggerDirtyData();
    emit fragmentShaderChanged(fragmentShader);
}

void Renderable::setGeometryShader(ShaderBuilder *geometryShader)
{
    if (m_geometryShader == geometryShader)
        return;
    if(m_geometryShader) {
        disconnect(m_geometryShader, 0, this, 0);
    }
    m_geometryShader = geometryShader;
    connect(m_geometryShader, &ShaderBuilder::finalShaderChanged, this, &Renderable::triggerDirtyShaders);
    connect(m_geometryShader, &ShaderBuilder::uniformsChanged, this, &Renderable::triggerDirtyData);
    triggerDirtyShaders();
    triggerDirtyData();
    emit geometryShaderChanged(geometryShader);
}

void Renderable::setVertexShader(ShaderBuilder *vertexShader)
{
    if (m_vertexShader == vertexShader)
        return;
    if(m_vertexShader) {
        disconnect(m_vertexShader, 0, this, 0);
    }
    m_vertexShader = vertexShader;
    connect(m_vertexShader, &ShaderBuilder::finalShaderChanged, this, &Renderable::triggerDirtyShaders);
    connect(m_vertexShader, &ShaderBuilder::uniformsChanged, this, &Renderable::triggerDirtyData);
    triggerDirtyShaders();
    triggerDirtyData();
    emit vertexShaderChanged(vertexShader);
}

void Renderable::setDirtyData(bool hasDirtyData)
{
    if (m_hasDirtyData == hasDirtyData)
        return;

    m_hasDirtyData = hasDirtyData;
    emit hasDirtyDataChanged(hasDirtyData);
}

void Renderable::setDirtyShaders(bool hasDirtyShaders)
{
    if (m_hasDirtyShaders == hasDirtyShaders)
        return;

    m_hasDirtyShaders = hasDirtyShaders;
    emit hasDirtyShadersChanged(hasDirtyShaders);
}

RenderableRenderer::RenderableRenderer()
{
    m_elapsedTime.start();
}

void RenderableRenderer::generateVBOs()
{
    if(m_vboCount>0) {
        m_vboIds.resize(m_vboCount);
        glFunctions()->glGenBuffers(m_vboCount, &m_vboIds.front());
    }
    m_vboIds.resize(m_vboCount);
    glFunctions()->glGenBuffers(m_vboCount, &m_vboIds.front());
    m_vao = new QOpenGLVertexArrayObject(this);
    m_vao->create();
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
        m_geometryShaderBase.clear();

        if(!QOpenGLContext::currentContext()->isOpenGLES()) {
            addShaderCodeToBase(QOpenGLShader::Fragment, generateGLSLHeader());
            addShaderCodeToBase(QOpenGLShader::Geometry, generateGLSLHeader());
            addShaderCodeToBase(QOpenGLShader::Vertex, generateGLSLHeader());
        }

        addShaderCodeToBase(QOpenGLShader::Fragment, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/default.glsl"));
        addShaderCodeToBase(QOpenGLShader::Fragment, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/defaultfragment.glsl"));
        addShaderCodeToBase(QOpenGLShader::Geometry, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/default.glsl"));
        addShaderCodeToBase(QOpenGLShader::Geometry, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/defaultgeometry.glsl"));
        addShaderCodeToBase(QOpenGLShader::Vertex, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/default.glsl"));
        addShaderCodeToBase(QOpenGLShader::Vertex, contentFromFile(":/org.compphys.SimVis/shadereffects/shaders/defaultvertex.glsl"));

        for(ShaderEffect *shaderEffect : m_shaderEffects) {
            if(shaderEffect->enabled()) {
                // Defines must come before library
                addShaderCodeToBase(QOpenGLShader::Fragment, shaderEffect->fragmentShaderDefines());
                addShaderCodeToBase(QOpenGLShader::Fragment, shaderEffect->fragmentShaderLibrary());
                addShaderCodeToBase(QOpenGLShader::Vertex, shaderEffect->vertexShaderDefines());
                addShaderCodeToBase(QOpenGLShader::Vertex, shaderEffect->vertexShaderLibrary());
                addShaderCodeToBase(QOpenGLShader::Geometry, shaderEffect->geometryShaderDefines());
                addShaderCodeToBase(QOpenGLShader::Geometry, shaderEffect->geometryShaderLibrary());
            }
        }

        setShaderFromSourceCode(QOpenGLShader::Vertex, m_vertexShaderSource);
        if(!m_geometryShaderSource.isEmpty()) {
            setShaderFromSourceCode(QOpenGLShader::Geometry, m_geometryShaderSource);
        }
        setShaderFromSourceCode(QOpenGLShader::Fragment, m_fragmentShaderSource);

        beforeLinkProgram();
        m_program.link();
        m_shadersDirty = false;
    }
    if(!m_program.isLinked()) {
        return;
    }

    m_program.bind();
    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    m_program.setUniformValue("cp_modelViewProjectionMatrix", modelViewProjectionMatrix);
    m_program.setUniformValue("cp_modelViewMatrix", m_modelViewMatrix);
    m_program.setUniformValue("cp_projectionMatrix", m_projectionMatrix);
    m_program.setUniformValue("cp_modelViewMatrixInverse", m_modelViewMatrixInverse);
    m_program.setUniformValue("cp_normalMatrix", m_modelViewMatrix.normalMatrix());

    m_program.setUniformValue("cp_projectionMatrixInverse", m_projectionMatrixInverse);
    m_program.setUniformValue("cp_viewVector", m_viewVector.normalized());
    m_program.setUniformValue("cp_rightVector", m_rightVector.normalized());
    m_program.setUniformValue("cp_upVector", m_upVector.normalized());
    m_program.setUniformValue("cp_cameraPosition", m_cameraPosition);
    m_program.setUniformValue("cp_time", float(m_elapsedTime.elapsed()*1e-3));
    GLint numberOfLights = 0;
    for(ShaderEffect *shaderEffect : m_shaderEffects) {
        if(shaderEffect->enabled()) {
            Light* light = qobject_cast<Light*>(shaderEffect);
            if(light) {
                light->setLightId(numberOfLights);
                numberOfLights++;
            }
            shaderEffect->beforeRendering(m_program);
        }
    }
    m_program.setUniformValue("cp_numberOfLights", numberOfLights);
    float oneOverNumberOfLights = (numberOfLights > 0) ? 1.0/numberOfLights : 10000.0;
    m_program.setUniformValue("cp_oneOverNumberOfLights", oneOverNumberOfLights);

    for(QString uniformName : m_uniforms.keys()) {
        QVariant value = m_uniforms.value(uniformName);
        QByteArray nameArray = uniformName.toUtf8();
        const char* name = nameArray.constData();
        switch(value.type()) {
        case QVariant::Bool:
            program().setUniformValue(name, value.toBool());
            break;
        case QVariant::Int:
            program().setUniformValue(name, value.toFloat());
            break;
        case QVariant::Double:
            program().setUniformValue(name, value.toFloat());
            break;
        case QVariant::Vector2D:
            program().setUniformValue(name, value.value<QVector2D>());
            break;
        case QVariant::Vector3D:
            program().setUniformValue(name, value.value<QVector3D>());
            break;
        case QVariant::Vector4D:
            program().setUniformValue(name, value.value<QVector4D>());
            break;
        case QVariant::Color:
            program().setUniformValue(name, value.value<QColor>());
            break;
        case QVariant::String:
            program().setUniformValue(name, QColor(value.toString()));
            break;
        default:
            qWarning() << "Cannot set uniform value because the type is unknown:" << value;
            break;
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
    } else if(type == QOpenGLShader::Geometry) {
        fullShaderCode = m_geometryShaderBase;
    } else {
        qDebug() << "Shaders of this type aren't supported yet.";
        return;
    }

    fullShaderCode.append(shaderCode);
    removeShader(type);
    // sqDebug() << "Shader: " << fullShaderCode;

    QString fileName = "/tmp/currentshader.";
    if(type == QOpenGLShader::Fragment) {
        fileName += ".fsh";
    }
    if(type == QOpenGLShader::Geometry) {
        fileName += ".gsh";
    }
    if(type == QOpenGLShader::Vertex) {
        fileName += ".vsh";
    }
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    QTextStream out(&file);
    out << fullShaderCode;
    m_program.addShaderFromSourceCode(type, fullShaderCode);
}

void RenderableRenderer::setShaderFromSourceFile(QOpenGLShader::ShaderType type, QString fileName) {
    QString shaderCode = contentFromFile(fileName);
    setShaderFromSourceCode(type, shaderCode);
}

bool RenderableRenderer::isGeometryShadersSupported()
{
    return QOpenGLShader::hasOpenGLShaders(QOpenGLShader::Geometry, QOpenGLContext::currentContext());
}

QString RenderableRenderer::generateGLSLHeader()
{
    /*
     * Based on http://stackoverflow.com/questions/19021567/finding-supported-glsl-version
     */
    QPair<int,int> openGLVersion = QOpenGLContext::currentContext()->format().version();

    QString header;
    if(openGLVersion==qMakePair(2,0)) header = "#version 110\n";
    if(openGLVersion==qMakePair(2,1)) header = "#version 120\n";
    if(openGLVersion==qMakePair(3,0)) header = "#version 130\n";
    if(openGLVersion==qMakePair(3,1)) header = "#version 140\n";
    if(openGLVersion==qMakePair(3,2)) header = "#version 150\n";
    if(openGLVersion>=qMakePair(3,3)) header = QString("#version %1%2%3\n").arg(openGLVersion.first).arg(openGLVersion.second).arg(0);
    return header;
}

void RenderableRenderer::addShaderCodeToBase(QOpenGLShader::ShaderType type, QString shaderCode) {
    if(type == QOpenGLShader::Fragment) m_fragmentShaderBase.append(shaderCode);
    else if(type == QOpenGLShader::Vertex) m_vertexShaderBase.append(shaderCode);
    else if(type == QOpenGLShader::Geometry) m_geometryShaderBase.append(shaderCode);
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
