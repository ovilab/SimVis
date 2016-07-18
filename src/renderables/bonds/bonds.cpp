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

QColor Bonds::color() const
{
    return m_color;
}

void Bonds::setData(QVector<BondData> &data)
{
    m_data = data;
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

void Bonds::setColor(QColor color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(color);
}

BondsRenderer::BondsRenderer()
{
    m_numberOfVBOs = 2;
}

void BondsRenderer::synchronize(Renderable* renderer)
{
    Bonds* bonds= static_cast<Bonds*>(renderer);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }
    if(m_vertexCount == 0) {
        m_indexCount = 0;
        m_vertexCount = 0;
    }

    uploadVBOs(bonds);
    m_vertexCount = bonds->m_vertices.size();
    m_color = QVector3D(bonds->color().redF(), bonds->color().greenF(), bonds->color().blueF());
}

void BondsRenderer::uploadVBOs(Bonds* bonds)
{
    if(!bonds->dirty()) return;
    QVector<BondData> &dataVector = bonds->m_data;
    QVector<GLuint> &indices = bonds->m_indices;
    QVector<BondsVBOData>& vertices = bonds->m_vertices;
    vertices.resize(dataVector.size()*6); // 6 vertices per bond
    indices.resize(dataVector.size()*12); // 4 triangles per bond

    for(int i=0; i<dataVector.size(); i++) {
        BondData &data = dataVector[i];

        for(int k=0; k<6; k++) {
            vertices[6*i+k].vertex1Position = data.vertex1Position;
            vertices[6*i+k].vertex2Position = data.vertex2Position;
            vertices[6*i+k].radius1 = data.radius1;
            vertices[6*i+k].radius2 = data.radius2;
            vertices[6*i+k].sphereRadius1 = data.sphereRadius1;
            vertices[6*i+k].sphereRadius2 = data.sphereRadius2;
            vertices[6*i+k].vertexId = k;
        }

        indices [12*i + 0] = 6*i+1;
        indices [12*i + 1] = 6*i+1;
        indices [12*i + 2] = 6*i+0;

        indices [12*i + 3] = 6*i+2;
        indices [12*i + 4] = 6*i+3;
        indices [12*i + 5] = 6*i+1;

        indices [12*i + 6] = 6*i+4;
        indices [12*i + 7] = 6*i+3;
        indices [12*i + 8] = 6*i+2;

        indices [12*i + 9] = 6*i+4;
        indices [12*i + 10] = 6*i+5;
        indices [12*i + 11] = 6*i+3;
    }

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

    // Transfer index data to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
    m_indexCount = indices.size();
}

void BondsRenderer::beforeLinkProgram() {
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/bonds/bonds.vsh");
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/bonds/bonds.fsh");
}

void BondsRenderer::render()
{
    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    program().setUniformValue("color", m_color);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertex1Position = program().attributeLocation("vertex1Position");
    int vertex2Position = program().attributeLocation("vertex2Position");
    int radius1 = program().attributeLocation("radius1");
    int radius2 = program().attributeLocation("radius2");
    int sphereRadius1 = program().attributeLocation("sphereRadius1");
    int sphereRadius2 = program().attributeLocation("sphereRadius2");
    int vertexId = program().attributeLocation("vertexId");

    // Offset for position
    quintptr offset = 0;

    program().enableAttributeArray(vertex1Position);
    glFunctions()->glVertexAttribPointer(vertex1Position, 3, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(QVector3D);
    program().enableAttributeArray(vertex2Position);
    glFunctions()->glVertexAttribPointer(vertex2Position, 3, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(QVector3D);

    program().enableAttributeArray(radius1);
    glFunctions()->glVertexAttribPointer(radius1, 1, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    program().enableAttributeArray(radius2);
    glFunctions()->glVertexAttribPointer(radius2, 1, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    program().enableAttributeArray(sphereRadius1);
    glFunctions()->glVertexAttribPointer(sphereRadius1, 1, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    program().enableAttributeArray(sphereRadius2);
    glFunctions()->glVertexAttribPointer(sphereRadius2, 1, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

    offset += sizeof(GLfloat);

    program().enableAttributeArray(vertexId);
    glFunctions()->glVertexAttribPointer(vertexId, 1, GL_FLOAT, GL_FALSE, sizeof(BondsVBOData), (const void *)offset);

//    offset += sizeof(GLfloat);

    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);
    glFunctions()->glDisable(GL_CULL_FACE);

    // glFunctions()->glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

    glFunctions()->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glFunctions()->glDisable(GL_DEPTH_TEST);

    program().disableAttributeArray(vertex1Position);
    program().disableAttributeArray(vertex2Position);
    program().disableAttributeArray(radius1);
    program().disableAttributeArray(radius2);
    program().disableAttributeArray(sphereRadius1);
    program().disableAttributeArray(sphereRadius2);
    program().disableAttributeArray(vertexId);

}
