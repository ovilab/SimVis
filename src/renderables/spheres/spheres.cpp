#include "spheres.h"

#include <QFileInfo>
#include <QColor>
#include <QRgb>
#include <cmath>
#include <QThread>
#include <QPainter>

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
bool Spheres::dirty() const
{
    return m_dirty;
}

void Spheres::setData(const QVector<SphereData> &data)
{
    m_data = data;
}

void Spheres::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
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

}

void SpheresRenderer::synchronize(Renderable* renderer)
{
    Spheres* spheres= static_cast<Spheres*>(renderer);

    m_upVector = spheres->camera()->upVector().normalized();
    m_viewVector = spheres->camera()->viewVector().normalized();
    m_rightVector = QVector3D::crossProduct(m_viewVector, m_upVector);

    if(!m_isInitialized) {
        if(geometryShaderIsSupported()) m_numberOfVBOs = 1;
        else m_numberOfVBOs = 2;
        generateVBOs();
        m_isInitialized = true;
    }

    uploadVBOs(spheres);
}

void SpheresRenderer::uploadVBONoGeometryShader(Spheres* spheres) {
    QVector<SphereData> &data = spheres->m_data;
    if(data.size() == 0) {
        m_indexCount = 0;
        m_vertexCount = 0;
        return;
    }

    if(!spheres->dirty()) {
        return;
    }
    QVector<SphereNoGeometryShaderVBOData>& vertices = spheres->m_verticesNoGeometryShader;
    QVector<GLuint>& indices = spheres->m_indices;
    int numberOfVertices = data.size()*4;
    vertices.resize(numberOfVertices);
    indices.resize(6*data.size());

    for(auto i=0; i<data.size(); i++) {
        QVector3D &position = data[i].position;
        float scale = data[i].scale;

        vertices[4*i + 0].scale = scale;
        vertices[4*i + 0].vertexId = 0;
        vertices[4*i + 0].position = position;

        vertices[4*i + 1].scale = scale;
        vertices[4*i + 1].vertexId = 1;
        vertices[4*i + 1].position = position;

        vertices[4*i + 2].scale = scale;
        vertices[4*i + 2].vertexId = 2;
        vertices[4*i + 2].position = position;

        vertices[4*i + 3].scale = scale;
        vertices[4*i + 3].vertexId = 3;
        vertices[4*i + 3].position = position;

        vertices[4*i + 0].color = data[i].color;
        vertices[4*i + 1].color = data[i].color;
        vertices[4*i + 2].color = data[i].color;
        vertices[4*i + 3].color = data[i].color;

        indices [6*i + 0] = 4*i+0;
        indices [6*i + 1] = 4*i+1;
        indices [6*i + 2] = 4*i+2;

        indices [6*i + 3] = 4*i+1;
        indices [6*i + 4] = 4*i+3;
        indices [6*i + 5] = 4*i+2;
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
    QVector<SphereData> &data = spheres->m_data;
    QVector<SphereGeometryShaderVBOData>& vertices = spheres->m_verticesGeometryShader;

    if(data.size() < 1 || !spheres->dirty()) {
        return;
    }

    int numberOfVertices = data.size();
    vertices.resize(numberOfVertices);
    for(auto i=0; i<numberOfVertices; i++) {
        vertices[i].position = data[i].position;
        vertices[i].color = data[i].color;
        vertices[i].scale = data[i].scale;
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
    if(geometryShaderIsSupported()) uploadVBOGeometryShader(spheres);
    else uploadVBONoGeometryShader(spheres);
}

void SpheresRenderer::beforeLinkProgram() {
    if(geometryShaderIsSupported()) {
        setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/spheres/spheresgs.vsh");
        setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/spheres/spheresgs.fsh");
        setShaderFromSourceFile(QOpenGLShader::Geometry, ":/org.compphys.SimVis/renderables/spheres/spheres.gsh");
    } else {
        setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/spheres/spheres.vsh");
        setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/spheres/spheres.fsh");
    }
}

void SpheresRenderer::renderNoGeometryShader() {
    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    QVector3D upMinusRightHalf = (m_upVector - m_rightVector)*0.5;
    QVector3D upPlusRightHalf = (m_upVector + m_rightVector)*0.5;
    program().setUniformValue("cp_upMinusRightHalf", upMinusRightHalf);
    program().setUniformValue("cp_upPlusRightHalf", upPlusRightHalf);

    // Offset for position
    quintptr offset = 0;

    int vertexPositionLocation = program().attributeLocation("vertexPosition");
    int scaleLocation = program().attributeLocation("scale");
    int vertexIdLocation = program().attributeLocation("vertexId");
    int colorLocation = program().attributeLocation("col");

    // Tell OpenGL programmable pipeline how to locate vertex position data
    program().enableAttributeArray(scaleLocation);
    glFunctions()->glVertexAttribPointer(scaleLocation, 1, GL_FLOAT, GL_FALSE, sizeof(SphereNoGeometryShaderVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    program().enableAttributeArray(vertexIdLocation);
    glFunctions()->glVertexAttribPointer(vertexIdLocation, 1, GL_FLOAT, GL_FALSE, sizeof(SphereNoGeometryShaderVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    // Tell OpenGL programmable pipeline how to locate vertex position data

    program().enableAttributeArray(vertexPositionLocation);
    glFunctions()->glVertexAttribPointer(vertexPositionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereNoGeometryShaderVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex color data

    program().enableAttributeArray(colorLocation);
    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereNoGeometryShaderVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data

    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);
    glFunctions()->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glFunctions()->glDisable(GL_DEPTH_TEST);

    program().disableAttributeArray(vertexPositionLocation);
    program().disableAttributeArray(colorLocation);
    program().disableAttributeArray(scaleLocation);
    program().disableAttributeArray(vertexIdLocation);
}

void SpheresRenderer::renderGeometryShader() {
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());

    m_vao->bind();

    int positionLocation = 0;
    int colorLocation = 1;
    int scaleLocation = 2;

    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use
    quintptr offset = 0;

    program().enableAttributeArray(positionLocation);
    glFunctions()->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereGeometryShaderVBOData), (const void *)offset);
    offset += sizeof(QVector3D);

    program().enableAttributeArray(colorLocation);
    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereGeometryShaderVBOData), (const void *)offset);
    offset += sizeof(QVector3D);

    program().enableAttributeArray(scaleLocation);
    glFunctions()->glVertexAttribPointer(scaleLocation, 1, GL_FLOAT, GL_FALSE, sizeof(SphereGeometryShaderVBOData), (const void *)offset);
    glFunctions()->glDisable(GL_CULL_FACE);
    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);

    glDrawArrays(GL_POINTS, 0, m_vertexCount);

    program().disableAttributeArray(positionLocation);
    program().disableAttributeArray(colorLocation);
    program().disableAttributeArray(scaleLocation);
}

void SpheresRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }
    if(geometryShaderIsSupported()) renderGeometryShader();
    else renderNoGeometryShader();

}
