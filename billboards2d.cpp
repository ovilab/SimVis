#include "billboards2d.h"

#include <QFileInfo>
#include <QColor>
#include <QRgb>
#include <cmath>
#include <QThread>

using std::vector;

Billboards2D::Billboards2D()
{
    m_color = QVector3D(1.0, 1.0, 1.0);
}

Billboards2D::~Billboards2D()
{

}

void Billboards2DRenderer::uploadTexture(QString filename)
{
    QFileInfo info(filename);
    if(!info.exists()) {
        qWarning() << "File " << filename << " does not exist. Aborting!";
        return;
    }

    QImage img = QImage(filename).mirrored();
    m_texture = new QOpenGLTexture(img);

    m_isTextureUploaded = true;
}

QVector3D Billboards2D::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

std::vector<QVector2D> &Billboards2D::positions()
{
    return m_positions;
}

std::vector<float> &Billboards2D::rotations()
{
    return m_rotations;
}
float Billboards2D::scale() const
{
    return m_scale;
}

void Billboards2D::setScale(float scale)
{
    m_scale = scale;
}
QVector3D Billboards2D::color() const
{
    return m_color;
}

void Billboards2D::setColor(const QColor &color)
{
    m_color = vectorFromColor(color);
}

RenderableRenderer *Billboards2D::createRenderer()
{
    return new Billboards2DRenderer();
}

Billboards2DRenderer::Billboards2DRenderer()
{
    m_numberOfVBOs = 2;
}

void Billboards2DRenderer::synchronize(Renderable* renderer)
{
    Billboards2D* billboards = static_cast<Billboards2D*>(renderer);
    if(!m_isInitialized) {
        generateVBOs();
        m_isInitialized = true;
    }
    if(!m_isTextureUploaded) {
        uploadTexture(billboards->texture());
    }
    uploadVBOs(billboards);

    m_vertexCount = billboards->m_vertices.size();
    m_indexCount = billboards->m_indices.size();
}

void Billboards2DRenderer::uploadVBOs(Billboards2D* billboards)
{
    double scale = billboards->scale();
    std::vector<QVector2D>& positions = billboards->m_positions;
    std::vector<Billboard2DVBOData>& vertices = billboards->m_vertices;
    std::vector<GLuint>& indices = billboards->m_indices;
    QVector3D& color = billboards->m_color;
    std::vector<float>& rotations = billboards->m_rotations;

    QVector2D right;
    right.setX(1.0);
    right.setY(0.0);
    QVector2D up;
    up.setX(0.0);
    up.setY(1.0);

    QVector2D ul = (0.5*up - 0.5*right)*scale;
    QVector2D ur = (0.5*up + 0.5*right)*scale;
    QVector2D dl = (-0.5*up - 0.5*right)*scale;
    QVector2D dr = (-0.5*up + 0.5*right)*scale;

    int numberOfVertices = positions.size()*4;
    vertices.resize(numberOfVertices);
    indices.resize(6*positions.size());

    QVector3D normalColor = color;

    for(unsigned int i=0; i<positions.size(); i++) {
        QVector2D &position = positions[i];
        float rotation = 0;
        if(rotations.size() > 0) {
            if(rotations.size() == 1) {
                rotation = rotations[0];
            } else {
                rotation = rotations[i];
            }
        }
        float cosTheta = cos(rotation);
        float sinTheta = sin(rotation);

        vertices[4*i + 0].position = position;
        vertices[4*i + 0].position[0] += dl[0]*cosTheta - dl[1]*sinTheta;
        vertices[4*i + 0].position[1] += dl[0]*sinTheta + dl[1]*cosTheta;
        vertices[4*i + 0].textureCoord= QVector2D(0,1);

        vertices[4*i + 1].position = position;
        vertices[4*i + 1].position[0] += dr[0]*cosTheta - dr[1]*sinTheta;
        vertices[4*i + 1].position[1] += dr[0]*sinTheta + dr[1]*cosTheta;
        vertices[4*i + 1].textureCoord= QVector2D(1,1);

        vertices[4*i + 2].position = position;
        vertices[4*i + 2].position[0] += ur[0]*cosTheta - ur[1]*sinTheta;
        vertices[4*i + 2].position[1] += ur[0]*sinTheta + ur[1]*cosTheta;
        vertices[4*i + 2].textureCoord= QVector2D(1,0);

        vertices[4*i + 3].position = position;
        vertices[4*i + 3].position[0] += ul[0]*cosTheta - ul[1]*sinTheta;
        vertices[4*i + 3].position[1] += ul[0]*sinTheta + ul[1]*cosTheta;
        vertices[4*i + 3].textureCoord= QVector2D(0,0);

        QVector3D color = normalColor;

        vertices[4*i + 0].color = color;
        vertices[4*i + 1].color = color;
        vertices[4*i + 2].color = color;
        vertices[4*i + 3].color = color;

        indices [6*i + 0] = 4*i+0;
        indices [6*i + 1] = 4*i+1;
        indices [6*i + 2] = 4*i+2;

        indices [6*i + 3] = 4*i+2;
        indices [6*i + 4] = 4*i+3;
        indices [6*i + 5] = 4*i+0;
    }

    // Transfer vertex data to VBO 0
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Billboard2DVBOData), &vertices[0], GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
}

void Billboards2D::setPositions(std::vector<QVector2D> &positions)
{
    m_positions = positions;
}

void Billboards2DRenderer::beforeLinkProgram() {
    program().addShaderFromSourceCode(QOpenGLShader::Vertex,
                                      "attribute highp vec2 a_position;\n"
                                      "attribute highp vec3 a_color;\n"
                                      "attribute highp vec2 a_texcoord;\n"
                                      "varying highp vec2 coords;\n"
                                      "varying highp float light;\n"
                                      "varying highp vec3 color;\n"
                                      "void main() {\n"
                                      "    gl_Position = vec4(a_position, 0.0, 1.0);\n"
                                      "    coords = a_texcoord;\n"
                                      "    color = a_color;\n"
                                      "}");

    program().addShaderFromSourceCode(QOpenGLShader::Fragment,
                                      "uniform sampler2D texture;\n"
                                      "varying highp vec2 coords;\n"
                                      "varying highp vec3 color;\n"
                                      "void main() {\n"
                                      "    gl_FragColor = texture2D(texture, coords.st)*vec4(color, 1.0);\n"
                                      // "    if(gl_FragColor.w==0.0) { discard; }\n"
                                      "}");
}

void Billboards2DRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program().attributeLocation("a_position");
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Billboard2DVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector2D);

    // Tell OpenGL programmable pipeline how to locate vertex color data
    int colorLocation = program().attributeLocation("a_color");
    program().enableAttributeArray(colorLocation);
    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Billboard2DVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program().attributeLocation("a_texcoord");
    program().enableAttributeArray(texcoordLocation);
    glFunctions()->glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Billboard2DVBOData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    m_texture->bind();
    glFunctions()->glDisable(GL_DEPTH_TEST);
    glFunctions()->glEnable(GL_BLEND);
    glFunctions()->glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFunctions()->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
    glFunctions()->glDisable(GL_BLEND);

    program().disableAttributeArray(vertexLocation);
    program().disableAttributeArray(colorLocation);
    program().disableAttributeArray(texcoordLocation);
}
