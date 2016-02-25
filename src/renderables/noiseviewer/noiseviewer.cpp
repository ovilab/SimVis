#include "noiseviewer.h"

void NoiseViewerRenderer::uploadVBO()
{
    m_numberOfVoxels = m_voxelsPerDimension*m_voxelsPerDimension*m_voxelsPerDimension;

    QVector<QVector3D> vertices;
    vertices.reserve(m_numberOfVoxels);
    float dr = m_scale / m_voxelsPerDimension;
    float min = -m_scale*0.5;

    for(int i=0; i<m_voxelsPerDimension; i++) {
        float x = min + i*dr;
        for(int j=0; j<m_voxelsPerDimension; j++) {
            float y = min + j*dr;
            for(int k=0; k<m_voxelsPerDimension; k++) {
                float z = min + k*dr;
                vertices.push_back(QVector3D(x,y,z));
            }
        }
    }

    // Transfer vertex data to VBO 0
    m_vao->bind();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, m_numberOfVoxels * sizeof(QVector3D), &vertices.front(), GL_STATIC_DRAW);
}


RenderableRenderer *NoiseViewer::createRenderer()
{
    return new NoiseViewerRenderer();
}

int NoiseViewer::voxelsPerDimension() const
{
    return m_voxelsPerDimension;
}

float NoiseViewer::scale() const
{
    return m_scale;
}

void NoiseViewer::setVoxelsPerDimension(int voxelsPerDimension)
{
    if (m_voxelsPerDimension == voxelsPerDimension)
        return;

    m_voxelsPerDimension = voxelsPerDimension;
    emit voxelsPerDimensionChanged(voxelsPerDimension);
}

void NoiseViewer::setScale(float scale)
{
    if (m_scale == scale)
        return;

    m_scale = scale;
    emit scaleChanged(scale);
}

void NoiseViewerRenderer::beforeLinkProgram()
{
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/noiseviewer/noiseviewer.vsh");
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/noiseviewer/noiseviewer.fsh");
    setShaderFromSourceFile(QOpenGLShader::Geometry, ":/org.compphys.SimVis/renderables/noiseviewer/noiseviewer.gsh");
}

void NoiseViewerRenderer::synchronize(Renderable *renderable)
{
    NoiseViewer *noiseViewer = qobject_cast<NoiseViewer*>(renderable);
    if(noiseViewer) {
        if(!m_isInitialized) {
            m_vboCount = 1;
            generateVBOs();
            noiseViewer->setVoxelsPerDimension(32);
            noiseViewer->setScale(10);
            m_voxelsPerDimension = noiseViewer->voxelsPerDimension();
            m_scale = noiseViewer->scale();
            uploadVBO();
            m_isInitialized = true;
        }
    }
}

void NoiseViewerRenderer::render()
{
    m_vao->bind();

    int positionLocation = 0;

    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use
    float dr = m_scale / m_voxelsPerDimension;
    program().setUniformValue("delta000", QVector3D());
    program().setUniformValue("delta001", QVector3D(0, 0, dr));
    program().setUniformValue("delta010", QVector3D(0, dr, 0));
    program().setUniformValue("delta011", QVector3D(0, dr, dr));
    program().setUniformValue("delta100", QVector3D(dr, 0, 0));
    program().setUniformValue("delta101", QVector3D(dr, 0, dr));
    program().setUniformValue("delta110", QVector3D(dr, dr, 0));
    program().setUniformValue("delta111", QVector3D(dr, dr, dr));
    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    program().setUniformValue("mvp_delta000", modelViewProjectionMatrix*QVector4D());
    program().setUniformValue("mvp_delta001", modelViewProjectionMatrix*QVector4D(0, 0, dr, 0.0));
    program().setUniformValue("mvp_delta010", modelViewProjectionMatrix*QVector4D(0, dr, 0, 0.0));
    program().setUniformValue("mvp_delta011", modelViewProjectionMatrix*QVector4D(0, dr, dr, 0.0));
    program().setUniformValue("mvp_delta100", modelViewProjectionMatrix*QVector4D(dr, 0, 0, 0.0));
    program().setUniformValue("mvp_delta101", modelViewProjectionMatrix*QVector4D(dr, 0, dr, 0.0));
    program().setUniformValue("mvp_delta110", modelViewProjectionMatrix*QVector4D(dr, dr, 0, 0.0));
    program().setUniformValue("mvp_delta111", modelViewProjectionMatrix*QVector4D(dr, dr, dr, 0.0));
    program().enableAttributeArray(positionLocation);
    glFunctions()->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);

    glFunctions()->glEnable(GL_CULL_FACE);
    glFunctions()->glCullFace(GL_BACK);
    glFunctions()->glDepthMask(GL_TRUE);
    glFunctions()->glEnable(GL_DEPTH_TEST);

    glFunctions()->glDrawArrays(GL_POINTS, 0, m_numberOfVoxels);

    program().disableAttributeArray(positionLocation);
    glFunctions()->glDisable(GL_CULL_FACE);
}
