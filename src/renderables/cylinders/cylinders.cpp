#include "cylinders.h"

Cylinders::Cylinders(QQuickItem *parent)
    : Renderable(parent)
{

}

Cylinders::~Cylinders()
{

}

CylindersRenderer *Cylinders::createRenderer()
{
    return new CylindersRenderer();
}

bool Cylinders::dirty() const
{
    return m_dirty;
}

float Cylinders::radius() const
{
    return m_radius;
}

void Cylinders::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
}

void Cylinders::setRadius(float radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    emit radiusChanged(radius);
}

void Cylinders::setCylinders(const QVector<CylinderVBOData> &vertices)
{
    m_cylinders = vertices;
}

CylindersRenderer::CylindersRenderer()
{
    m_vboCount = 1;
}

void CylindersRenderer::synchronize(Renderable* renderer)
{
    Cylinders* cylinders= static_cast<Cylinders*>(renderer);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }
    uploadVBOs(cylinders);
    m_radius = cylinders->radius();
    m_vertexCount = cylinders->m_cylinders.size();
}

void CylindersRenderer::uploadVBOs(Cylinders* cylinders)
{
    if(!cylinders->dirty()) {
        return;
    }
    QVector<CylinderVBOData>& vertices = cylinders->m_cylinders;
    if(vertices.size() < 1) {
        return;
    }
    int numberOfVertices = vertices.size();
    cylinders->setDirty(false);

    // Transfer vertex data to VBO 0
    m_vao->bind();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(CylinderVBOData), &vertices.front(), GL_STATIC_DRAW);
    m_vertexCount = vertices.size();
}

void CylindersRenderer::beforeLinkProgram() {
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/cylinders/cylinders.vsh");
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/cylinders/cylinders.fsh");
    setShaderFromSourceFile(QOpenGLShader::Geometry, ":/org.compphys.SimVis/renderables/cylinders/cylinders.gsh");
}

void CylindersRenderer::render()
{
    m_vao->bind();

    int vertex1Location = 0;
    int vertex2Location = 1;
    int radius1Location = 2;
    int radius2Location = 3;
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use
    program().setUniformValue("radius", m_radius);
    quintptr offset = 0;

    program().enableAttributeArray(vertex1Location);
    glFunctions()->glVertexAttribPointer(vertex1Location, 3, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
    offset += sizeof(QVector3D);

    program().enableAttributeArray(vertex2Location);
    glFunctions()->glVertexAttribPointer(vertex2Location, 3, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
    offset += sizeof(QVector3D);

    program().enableAttributeArray(radius1Location);
    glFunctions()->glVertexAttribPointer(radius1Location, 1, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
    offset += sizeof(float);

    program().enableAttributeArray(radius2Location);
    glFunctions()->glVertexAttribPointer(radius2Location, 1, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);

    glFunctions()->glDisable(GL_CULL_FACE);
    glFunctions()->glDrawArrays(GL_POINTS, 0, m_vertexCount);

    program().disableAttributeArray(vertex1Location);
    program().disableAttributeArray(vertex2Location);
    program().disableAttributeArray(radius1Location);
    program().disableAttributeArray(radius2Location);
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
}
