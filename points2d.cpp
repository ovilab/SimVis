#include "points2d.h"

Points2D::Points2D()
{

}

Points2D::~Points2D()
{

}

void Points2D::setPositions(QVector<QVector2D> &positions)
{
    m_vertices = positions;
}

QVector<QVector2D> &Points2D::positions()
{
    return m_vertices;
}

RenderableRenderer *Points2D::createRenderer()
{
    return new Points2DRenderer();
}

float Points2D::pointSize() const
{
    return m_pointSize;
}

QColor Points2D::color() const
{
    return m_color;
}

void Points2D::setPointSize(float arg)
{
    if (m_pointSize == arg)
        return;

    m_pointSize = arg;
    emit pointSizeChanged(arg);
}

void Points2D::setColor(QColor arg)
{
    if (m_color == arg)
        return;

    m_color = arg;
    emit colorChanged(arg);
}

Points2DRenderer::Points2DRenderer()
{
    m_numberOfVBOs = 1;
}

void Points2DRenderer::synchronize(Renderable *renderable)
{
    Points2D* points = static_cast<Points2D*>(renderable);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }

    uploadVBO(points);
    m_vertexCount = points->m_vertices.size();
    m_pointSize = points->m_pointSize;
    m_color = QVector4D(points->m_color.redF(), points->m_color.greenF(), points->m_color.blueF(), points->m_color.alphaF());
}

void Points2DRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program().attributeLocation("a_position");
    program().setUniformValue("color", m_color);
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(QVector2D), 0);

    // Draw cube geometry using indices from VBO 1
    glPointSize(m_pointSize);
    glFunctions()->glDrawArrays(GL_POINTS, 0, m_vertexCount);

    program().disableAttributeArray(vertexLocation);
}

void Points2DRenderer::uploadVBO(Points2D *points)
{
    // Transfer vertex data to VBO 0
    m_vertexCount = points->m_vertices.size();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(QVector2D), &points->m_vertices.front(), GL_STATIC_DRAW);
}

void Points2DRenderer::beforeLinkProgram()
{
    program().addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      "attribute highp vec4 a_position;\n"
                                      "void main() {\n"
                                      "    gl_Position = a_position;\n"
                                      "}");

    program().addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      "uniform vec4 color;"
                                      "void main() {\n"
                                      "    gl_FragColor = color;\n"
                                      "}");
}
