#include "lines.h"

Lines::Lines()
{

}


LinesRenderer *Lines::createRenderer()
{
    return new LinesRenderer();
}

LinesRenderer::LinesRenderer()
{
    m_vboCount = 1;
}

void LinesRenderer::synchronize(Renderable *renderable)
{
    Lines* lines = static_cast<Lines*>(renderable);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }

    uploadVBO(lines);
}

void LinesRenderer::uploadVBO(Lines *lines)
{
    if(lines->data.size() < 1) {
        return;
    }

    m_vao->bind();
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, lines->data.size() * sizeof(QVector3D), &lines->data.front(), GL_STATIC_DRAW);
    m_vertexCount = lines->data.size();
}

void LinesRenderer::beforeLinkProgram()
{
    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/lines/lines.vsh");
    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/lines/lines.fsh");
}

void LinesRenderer::render()
{
    QOpenGLFunctions funcs(QOpenGLContext::currentContext());

    m_vao->bind();

    int positionLocation = 0;

    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]); // Tell OpenGL which VBOs to use
    quintptr offset = 0;

    program().enableAttributeArray(positionLocation);
    glFunctions()->glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), (const void *)offset);


    glFunctions()->glDisable(GL_CULL_FACE);
    glFunctions()->glEnable(GL_DEPTH_TEST);
    glFunctions()->glDepthMask(GL_TRUE);

    glFunctions()->glDrawArrays(GL_LINES, 0, m_vertexCount);

    program().disableAttributeArray(positionLocation);
}
