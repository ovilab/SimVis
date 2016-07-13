#include "bonds.h"

Bonds::Bonds(QQuickItem *parent)
    : Renderable(parent)
{

}

Bonds::~Bonds()
{

}

BondsRenderer *Bonds::createRenderer()
{
    return new BondsRenderer();
}

bool Bonds::dirty() const
{
    return m_dirty;
}

QVector<BondsVBOData> &Bonds::vertices() { return m_vertices; }

float Bonds::radius() const
{
    return m_radius;
}

void Bonds::setDirty(bool dirty)
{
    if (m_dirty == dirty)
        return;

    m_dirty = dirty;
    emit dirtyChanged(dirty);
}

void Bonds::setRadius(float radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    emit radiusChanged(radius);
}

BondsRenderer::BondsRenderer()
{
    m_numberOfVBOs = 1;
}

void BondsRenderer::synchronize(Renderable* renderer)
{
    Bonds* bonds= static_cast<Bonds*>(renderer);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }
    uploadVBOs(bonds);
    m_vertexCount = bonds->m_vertices.size();
}

void BondsRenderer::uploadVBOs(Bonds* bonds)
{
    if(!bonds->dirty()) return;
    QVector<BondsVBOData>& vertices = bonds->m_vertices;
    if(vertices.size() < 1) {
        return;
    }
    int numberOfVertices = vertices.size();
    bonds->setDirty(false);

    // Transfer vertex data to VBO 0
    m_vao->bind();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, numberOfVertices * sizeof(BondsVBOData), &vertices.front(), GL_STATIC_DRAW);
    m_vertexCount = vertices.size();
}

void BondsRenderer::beforeLinkProgram() {
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/bonds/bonds.vsh");
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/bonds/bonds.fsh");
}

void BondsRenderer::render()
{
//    QOpenGLFunctions funcs(QOpenGLContext::currentContext());
//    m_vao->bind();

//    int vertex1Location = 0;
//    int vertex2Location = 1;
//    int radius1Location = 2;
//    int radius2Location = 3;
//    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use
//    program().setUniformValue("radius", m_radius);
//    quintptr offset = 0;

//    program().enableAttributeArray(vertex1Location);
//    glFunctions()->glVertexAttribPointer(vertex1Location, 3, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
//    offset += sizeof(QVector3D);

//    program().enableAttributeArray(vertex2Location);
//    glFunctions()->glVertexAttribPointer(vertex2Location, 3, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
//    offset += sizeof(QVector3D);

//    program().enableAttributeArray(radius1Location);
//    glFunctions()->glVertexAttribPointer(radius1Location, 1, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);
//    offset += sizeof(float);

//    program().enableAttributeArray(radius2Location);
//    glFunctions()->glVertexAttribPointer(radius2Location, 1, GL_FLOAT, GL_FALSE, sizeof(CylinderVBOData), (const void *)offset);

//    funcs.glDisable(GL_CULL_FACE);
//    glDrawArrays(GL_LINES, 0, m_vertexCount);

//    program().disableAttributeArray(vertex1Location);
//    program().disableAttributeArray(vertex2Location);
//    program().disableAttributeArray(radius1Location);
//    program().disableAttributeArray(radius2Location);

}
