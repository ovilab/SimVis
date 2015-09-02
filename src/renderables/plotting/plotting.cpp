#include "plotting.h"

Plotting::Plotting()
{

}

Plotting::~Plotting()
{

}

RenderableRenderer *Plotting::createRenderer()
{
    return new PlottingRenderer();
}

void Plotting::setPositions(QVector<QVector3D> &positions)
{
    m_vertices = positions;
}

QVector<QVector3D> &Plotting::positions()
{
    return m_vertices;
}

PlottingRenderer::PlottingRenderer()
{
    m_numberOfVBOs = 1;
}

void PlottingRenderer::synchronize(Renderable *renderable)
{
    Plotting* plotting = static_cast<Plotting*>(renderable);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }

    uploadVBO(plotting);
    m_vertexCount = plotting->m_vertices.size();
}

void PlottingRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    program().setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
    program().setUniformValue("pointSize", 10);

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program().attributeLocation("a_position");
    program().setUniformValue("color", m_color);
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), 0);

    // Draw cube geometry using indices from VBO 1
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    glEnable(GL_PROGRAM_POINT_SIZE);
#endif
    glDrawArrays(GL_POINTS, 0, m_vertexCount);

    program().disableAttributeArray(vertexLocation);
}

void PlottingRenderer::uploadVBO(Plotting *plotting)
{
    if(plotting->m_vertices.size() < 1) {
        return;
    }
    // Transfer vertex data to VBO 0
    m_vertexCount = plotting->m_vertices.size();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(QVector3D), &plotting->m_vertices.front(), GL_STATIC_DRAW);
}

void PlottingRenderer::beforeLinkProgram()
{
    setShaderFromSourceCode(QOpenGLShader::Vertex,
                            "uniform highp float pointSize;\n"
                            "attribute highp vec4 a_position;\n"
                            "void main() {\n"
                            "    gl_PointSize = 10.0;\n"
                            "    gl_Position = cp_modelViewProjectionMatrix*a_position;\n"
                            "}");

    setShaderFromSourceCode(QOpenGLShader::Fragment,
                            "uniform highp vec4 color;"
                            "void main() {\n"
                            "    gl_FragColor = color;\n"
                            "}");
}
