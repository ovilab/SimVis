#include "trianglecollection.h"

TriangleCollection::TriangleCollection()
{

}



RenderableRenderer *TriangleCollection::createRenderer()
{
    return new TriangleCollectionRenderer();
}

void TriangleCollectionRenderer::beforeLinkProgram()
{
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/trianglecollection/trianglecollection.fsh");
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/trianglecollection/trianglecollection.vsh");
}

void TriangleCollectionRenderer::synchronize(Renderable *renderable)
{
    TriangleCollection* triangleCollection = static_cast<TriangleCollection*>(renderable);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }

    if(triangleCollection->dirty) {
        uploadVBO(triangleCollection);
        triangleCollection->dirty = false;
    }
}

void TriangleCollectionRenderer::render()
{
    using namespace SimVis;
    if(numberOfTriangles == 0) {
        return;
    }
    m_vao->bind();

    int positionLocation = 0;
    int normalLocation = 1;
    int colorLocation = 2;

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    program().enableAttributeArray(positionLocation);
    glFunctions()->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleCollectionVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    program().enableAttributeArray(normalLocation);
    glFunctions()->glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleCollectionVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    program().enableAttributeArray(colorLocation);
    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(TriangleCollectionVBOData), (const void *)offset);

    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);
    glFunctions()->glEnable(GL_CULL_FACE);
    glFunctions()->glCullFace(GL_FRONT);

    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    program().setUniformValue("normalVectorSign", -1.0f);
    glDrawElements(GL_TRIANGLES, numberOfTriangles*3, GL_UNSIGNED_INT, 0);

    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[2]);
    program().setUniformValue("normalVectorSign", 1.0f);
    glDrawElements(GL_TRIANGLES, numberOfTriangles*3, GL_UNSIGNED_INT, 0);
    program().disableAttributeArray(positionLocation);
    program().disableAttributeArray(normalLocation);
    program().disableAttributeArray(colorLocation);
    glFunctions()->glDisable(GL_CULL_FACE);
}

void TriangleCollectionRenderer::uploadVBO(TriangleCollection *triangleCollection)
{
    using namespace SimVis;
    numberOfTriangles = triangleCollection->data.size()/3;
    if(triangleCollection->data.size() == 0) return;
    // Transfer vertex data to VBO 0
    m_vao->bind();

    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, triangleCollection->data.size() * sizeof(TriangleCollectionVBOData), &triangleCollection->data[0], GL_STATIC_DRAW);

    QVector<Triangle> triangles;
    triangles.resize(numberOfTriangles);
    for(int i=0; i<numberOfTriangles; i++) {
        triangles[i].vertexIndices[0] = 3*i+0;
        triangles[i].vertexIndices[1] = 3*i+1;
        triangles[i].vertexIndices[2] = 3*i+2;
    }

    // Transfer index data for front triangles to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0], GL_STATIC_DRAW);

    for(int i=0; i<numberOfTriangles; i++) {
        triangles[i].vertexIndices[0] = 3*i+0;
        triangles[i].vertexIndices[1] = 3*i+2;
        triangles[i].vertexIndices[2] = 3*i+1;
    }

    // Transfer index data for back triangles to VBO 2
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[2]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), &triangles[0], GL_STATIC_DRAW);
}
