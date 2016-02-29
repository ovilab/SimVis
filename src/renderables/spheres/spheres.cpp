#include "spheres.h"

#include <QFileInfo>
#include <QColor>
#include <QRgb>
#include <cmath>
#include <QThread>
#include <QPainter>
#include <QQmlEngine>
#include <QQmlContext>

#include "../../core/camera.h"

Spheres::Spheres(QQuickItem *parent)
    : Renderable(parent)
{
}

Spheres::~Spheres()
{

}

QVector3D Spheres::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}
QVector<float> &Spheres::scales()
{
    return m_scales;
}

void Spheres::setScales(const QVector<float> &scales)
{
    m_scales = scales;
}

bool Spheres::dirty() const
{
    return m_dirty;
}

ShaderBuilder *Spheres::fragmentShader() const
{
    return m_fragmentShader;
}

void Spheres::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
}

void Spheres::markShadersDirty() {
    m_shadersDirty = true;
}

void Spheres::markDirty()
{
    setDirty(true);
}

void Spheres::setFragmentShader(ShaderBuilder *fragmentShader)
{
    if (m_fragmentShader == fragmentShader)
        return;
    if(m_fragmentShader) {
        disconnect(m_fragmentShader, 0, this, 0);
    }
    m_fragmentShader = fragmentShader;
    connect(m_fragmentShader, &ShaderBuilder::finalShaderChanged, this, &Spheres::markShadersDirty);
    connect(m_fragmentShader, &ShaderBuilder::uniformsChanged, this, &Spheres::markDirty);
    markShadersDirty();
    markDirty();
    emit fragmentShaderChanged(fragmentShader);
}

QVector<QColor> &Spheres::colors()
{
    return m_colors;
}

void Spheres::setColors(const QVector<QColor> &colors)
{
    m_colors = colors;
}


QVector<QVector3D> &Spheres::positions()
{
    return m_positions;
}

float Spheres::scale() const
{
    return m_scale;
}

void Spheres::setScale(float scale)
{
    m_scale = scale;
}

QColor Spheres::color() const
{
    return m_color;
}

void Spheres::setColor(const QColor &color)
{
    m_color = color;
}

SpheresRenderer *Spheres::createRenderer()
{
    return new SpheresRenderer();
}

SpheresRenderer::SpheresRenderer()
{
    m_vboCount = 2;
}

void SpheresRenderer::synchronize(Renderable* renderer)
{
    Spheres* spheres = static_cast<Spheres*>(renderer);

    m_upVector = spheres->camera()->upVector().normalized();
    m_viewVector = spheres->camera()->viewVector().normalized();
    m_rightVector = QVector3D::crossProduct(m_viewVector, m_upVector);

    if(spheres->m_shadersDirty) {
        m_shadersDirty = true;
        m_fragmentShaderString = spheres->fragmentShader()->finalShader();
        spheres->m_shadersDirty = false;
    }

    m_uniforms = spheres->fragmentShader()->uniforms();

    if(!m_isInitialized) {
        if(geometryShaderIsSupported()) m_vboCount = 1;
        else m_vboCount = 2;
        generateVBOs();
        m_isInitialized = true;
    }

    uploadVBOs(spheres);
}

void SpheresRenderer::uploadVBONoGeometryShader(Spheres* spheres) {
    if(!spheres->dirty() || spheres->m_positions.size() == 0) {
        return;
    }
    float scale = spheres->scale();
    QVector<QVector3D>& positions = spheres->m_positions;
    QVector<QColor>& colors = spheres->m_colors;
    QVector<float>& scales = spheres->m_scales;
    QVector<SphereNoGeometryShaderVBOData>& vertices = spheres->m_verticesNoGeometryShader;
    QVector<GLuint>& indices = spheres->m_indices;
    QVector3D color = spheres->vectorFromColor(spheres->color());

    int numberOfVertices = positions.size()*4;
    vertices.resize(numberOfVertices);
    indices.resize(6*positions.size());
    bool individualColors = colors.size() == positions.size();
    bool individualScales = scales.size() == positions.size();
    for(auto i=0; i<positions.size(); i++) {
        QVector3D &position = positions[i];
        float additionalScale = scale;
        if(individualScales) {
            additionalScale *= scales[i];
        }

        vertices[4*i + 0].sphereId = i;
        vertices[4*i + 0].scale = additionalScale;
        vertices[4*i + 0].vertexId = 0;
        vertices[4*i + 0].position = position;
        vertices[4*i + 0].textureCoord= QVector2D(0,1);

        vertices[4*i + 1].sphereId = i;
        vertices[4*i + 1].scale = additionalScale;
        vertices[4*i + 1].vertexId = 1;
        vertices[4*i + 1].position = position;
        vertices[4*i + 1].textureCoord= QVector2D(1,1);

        vertices[4*i + 2].sphereId = i;
        vertices[4*i + 2].scale = additionalScale;
        vertices[4*i + 2].vertexId = 2;
        vertices[4*i + 2].position = position;
        vertices[4*i + 2].textureCoord= QVector2D(1,0);

        vertices[4*i + 3].sphereId = i;
        vertices[4*i + 3].scale = additionalScale;
        vertices[4*i + 3].vertexId = 3;
        vertices[4*i + 3].position = position;
        vertices[4*i + 3].textureCoord= QVector2D(0,0);
        if(individualColors) {
            color = QVector3D(colors[i].redF(), colors[i].greenF(), colors[i].blueF());
        }
        vertices[4*i + 0].color = color;
        vertices[4*i + 1].color = color;
        vertices[4*i + 2].color = color;
        vertices[4*i + 3].color = color;

        indices [6*i + 0] = 4*i+0;
        indices [6*i + 1] = 4*i+1;
        indices [6*i + 2] = 4*i+2;

        indices [6*i + 3] = 4*i+2;
        indices [6*i + 4] = 4*i+3;
        indices [6*i + 5] = 4*i+0;
    }

    spheres->setDirty(false);

    // Transfer vertex data to VBO 0
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SphereNoGeometryShaderVBOData), &vertices.front(), GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
    m_vertexCount = spheres->m_verticesNoGeometryShader.size();
    m_indexCount = spheres->m_indices.size();
}

void SpheresRenderer::uploadVBOGeometryShader(Spheres* spheres) {
    QVector<QVector3D>& positions = spheres->m_positions;
    QVector<QColor>& colors = spheres->m_colors;
    QVector<float>& scales = spheres->m_scales;
    QVector<SphereGeometryShaderVBOData>& vertices = spheres->m_verticesGeometryShader;

    if(positions.size() < 1 || !spheres->dirty()) {
        return;
    }

    int numberOfVertices = positions.size();
    vertices.resize(numberOfVertices);
    bool hasScales = scales.size();
    bool hasColors = colors.size();
    for(auto i=0; i<numberOfVertices; i++) {
        vertices[i].position = positions[i];
        if(hasColors) {
            vertices[i].color[0] = colors[i].redF();
            vertices[i].color[1] = colors[i].greenF();
            vertices[i].color[2] = colors[i].blueF();
        } else {
            vertices[i].color[0] = spheres->color().redF();
            vertices[i].color[1] = spheres->color().greenF();
            vertices[i].color[2] = spheres->color().blueF();
        }
        if(hasScales) {
            vertices[i].scale = scales[i]*spheres->scale()*0.5;
        } else {
            vertices[i].scale = spheres->scale();
        }
    }
    spheres->setDirty(false);

    if(vertices.size() < 1) {
        return;
    }

    // Transfer vertex data to VBO 0
    m_vao->bind();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(SphereGeometryShaderVBOData), &vertices.front(), GL_STATIC_DRAW);
    m_vertexCount = spheres->m_verticesGeometryShader.size();
}

void SpheresRenderer::uploadVBOs(Spheres* spheres)
{
    if(geometryShaderIsSupported()) {
        uploadVBOGeometryShader(spheres);
    } else {
        uploadVBONoGeometryShader(spheres);
    }
}

void Spheres::setPositions(QVector<QVector3D> &positions)
{
    m_positions = positions;
}

void SpheresRenderer::beforeLinkProgram() {
    if(geometryShaderIsSupported()) {
        setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/spheres/spheresgs.vsh");
        setShaderFromSourceFile(QOpenGLShader::Geometry, ":/org.compphys.SimVis/renderables/spheres/spheresgs.gsh");
        if(m_fragmentShaderString.isEmpty()) {
            setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/spheres/spheresgs.fsh");
        } else {
            setShaderFromSourceCode(QOpenGLShader::Fragment, m_fragmentShaderString);
        }
    } else {
        setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/spheres/spheres.vsh");
        if(m_fragmentShaderString.isEmpty()) {
            setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/spheres/spheres.fsh");
        } else {
            setShaderFromSourceCode(QOpenGLShader::Fragment, m_fragmentShaderString);
        }
    }
}

void SpheresRenderer::setUniforms() {
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
}

void SpheresRenderer::renderNoGeometryShader() {
    if(!program().isLinked()) {
        return;
    }

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    QVector3D upMinusRightHalf = (m_upVector - m_rightVector)*0.5;
    QVector3D upPlusRightHalf = (m_upVector + m_rightVector)*0.5;
    program().setUniformValue("cp_upMinusRightHalf", upMinusRightHalf);
    program().setUniformValue("cp_upPlusRightHalf", upPlusRightHalf);

    setUniforms();

    VertexAttributeArrayHelper<SphereNoGeometryShaderVBOData> helper(this);
    helper.addData(helper()->sphereId, "a_sphereId");
    helper.addData(helper()->scale, "a_scale");
    helper.addData(helper()->vertexId, "a_vertexId");
    helper.addData(helper()->position, "a_position");
    helper.addData(helper()->color, "a_color");
    helper.addData(helper()->textureCoord, "a_texCoord");

    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);
    glFunctions()->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glFunctions()->glDisable(GL_DEPTH_TEST);
}

void SpheresRenderer::renderGeometryShader() {
    if(!program().isLinked()) {
        return;
    }

    QOpenGLFunctions funcs(QOpenGLContext::currentContext());

    setUniforms();

    m_vao->bind();

    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use

    VertexAttributeArrayHelper<SphereGeometryShaderVBOData> helper(this);
    helper.addData(helper()->position, 0);
    helper.addData(helper()->color, 1);
    helper.addData(helper()->scale, 2);
    helper.addData(helper()->sphereId, 3);

    glFunctions()->glDisable(GL_CULL_FACE);
    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);

    glFunctions()->glDrawArrays(GL_POINTS, 0, m_vertexCount);
}

void SpheresRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }
    if(geometryShaderIsSupported()) {
        renderGeometryShader();
    } else {
        renderNoGeometryShader();
    }

}
