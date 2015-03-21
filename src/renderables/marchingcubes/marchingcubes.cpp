#include "marchingcubes.h"
#include <iostream>
using namespace std;

function<float (const QVector3D point)> MarchingCubes::scalarFieldEvaluator() const
{
    return m_scalarFieldEvaluator;
}

void MarchingCubes::setScalarFieldEvaluator(const function<float(const QVector3D pos)> &scalarFieldEvaluator)
{
    m_scalarFieldEvaluator = scalarFieldEvaluator;
    m_hasScalarField = true;
    setDirty(true);
}
MarchingCubes::MarchingCubes()
{

}

MarchingCubes::~MarchingCubes()
{

}

RenderableRenderer *MarchingCubes::createRenderer()
{
    return new MarchingCubesRenderer();
}

float MarchingCubes::threshold() const
{
    return m_threshold;
}

QVector3D MarchingCubes::min() const
{
    return m_min;
}

QVector3D MarchingCubes::max() const
{
    return m_max;
}

bool MarchingCubes::dirty() const
{
    return m_dirty;
}

QVector3D MarchingCubes::numVoxels() const
{
    return m_numVoxels;
}

bool MarchingCubes::hasScalarField() const
{
    return m_hasScalarField;
}

void MarchingCubes::setThreshold(float arg)
{
    if (m_threshold == arg)
        return;

    m_threshold = arg;
    setDirty(true);
    emit thresholdChanged(arg);
}

void MarchingCubes::setMin(QVector3D arg)
{
    if (m_min == arg)
        return;

    m_min = arg;
    setDirty(true);
    emit minChanged(arg);
}

void MarchingCubes::setMax(QVector3D arg)
{
    if (m_max == arg)
        return;

    m_max = arg;
    setDirty(true);
    emit maxChanged(arg);
}

void MarchingCubes::setDirty(bool arg)
{
    if (m_dirty == arg)
        return;

    m_dirty = arg;
    emit dirtyChanged(arg);
}

void MarchingCubes::setNumVoxels(QVector3D arg)
{
    if (m_numVoxels == arg)
        return;

    m_numVoxels = arg;
    setDirty(true);
    emit numVoxelsChanged(arg);
}

MarchingCubesRenderer::MarchingCubesRenderer()
{
    m_numberOfVBOs = 2;
}

void MarchingCubesRenderer::synchronize(Renderable *renderable)
{
    MarchingCubes* marchingCubes = static_cast<MarchingCubes*>(renderable);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }

    if(marchingCubes->dirty()) {
        if(marchingCubes->hasScalarField()) {
            m_generator.setScalarFieldEvaluator(marchingCubes->scalarFieldEvaluator());
            m_generator.generateSurface(marchingCubes->min(), marchingCubes->max(), marchingCubes->numVoxels(), marchingCubes->threshold());
            uploadVBOs();
        }
        marchingCubes->setDirty(false);
    }
}

void MarchingCubesRenderer::render()
{
    cout << "Rendering" << endl;
    if(m_indexCount == 0) {
        return;
    }

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    program().setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
    program().setUniformValue("lightPosition", QVector3D(5,0,0));

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program().attributeLocation("a_position");
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(MarchingCubesVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    vertexLocation = program().attributeLocation("a_normal");
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(MarchingCubesVBOData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    qDebug() << "Will draw triangles, num indices: " << m_indexCount;
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

    program().disableAttributeArray(vertexLocation);
}

void MarchingCubesRenderer::uploadVBOs()
{
    cout << "Uploading VBO" << endl;
    if(m_generator.m_data.size() == 0) return;
    // Transfer vertex data to VBO 0
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, m_generator.m_data.size() * sizeof(MarchingCubesVBOData), &m_generator.m_data[0], GL_STATIC_DRAW);
    qDebug() << "We have " << m_generator.m_data.size() << " vertices.";
    // Transfer index data to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_generator.m_triangles.size() * sizeof(Triangle), &m_generator.m_triangles[0], GL_STATIC_DRAW);
    qDebug() << "We have " << m_generator.m_triangles.size() << " triangles.";

    m_indexCount = 3*m_generator.m_triangles.size();
}

void MarchingCubesRenderer::beforeLinkProgram()
{
    program().addShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/marchingcubes/marchingcubes.vsh");
    program().addShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/marchingcubes/marchingcubes.fsh");
}
