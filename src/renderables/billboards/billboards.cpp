#include "billboards.h"

#include <QFileInfo>
#include <QColor>
#include <QRgb>
#include <cmath>
#include <QThread>

#include "../../core/camera.h"

Billboards::Billboards(QObject *parent)
    : Renderable(parent)
{

}

Billboards::~Billboards()
{

}

void BillboardsRenderer::uploadTexture(QString filename)
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

QVector3D Billboards::vectorFromColor(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

QVector<QVector3D> &Billboards::positions()
{
    return m_positions;
}

QVector<float> &Billboards::rotations()
{
    return m_rotations;
}
float Billboards::scale() const
{
    return m_scale;
}

void Billboards::setScale(float scale)
{
    m_scale = scale;
}
QVector3D Billboards::color() const
{
    return m_color;
}

void Billboards::setColor(const QColor &color)
{
    m_color = vectorFromColor(color);
}

RenderableRenderer *Billboards::createRenderer()
{
    return new BillboardsRenderer();
}

QString Billboards::texture() const
{
    return m_texture;
}

QColor Billboards::shadowColor() const
{
    return m_shadowColor;
}

void Billboards::setTexture(QString arg)
{
    if (m_texture == arg)
        return;

    m_texture = arg;
    emit textureChanged(arg);
}

void Billboards::setShadowColor(QColor arg)
{
    if (m_shadowColor == arg)
        return;

    m_shadowColor = arg;
    emit shadowColorChanged(arg);
}

BillboardsRenderer::BillboardsRenderer()
{
    m_numberOfVBOs = 2;
}

void BillboardsRenderer::synchronize(Renderable* renderer)
{
    Billboards* billboards = static_cast<Billboards*>(renderer);

    m_upVector = billboards->camera()->upVector().normalized();
    m_viewVector = billboards->camera()->viewVector().normalized();
    m_rightVector = QVector3D::crossProduct(m_viewVector, m_upVector);
    m_shadowColor = billboards->shadowColor();

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

void BillboardsRenderer::uploadVBOs(Billboards* billboards)
{
    if(billboards->m_positions.size() < 1) {
        return;
    }
    double scale = billboards->scale();
    QVector<QVector3D>& positions = billboards->m_positions;
    QVector<BillboardVBOData>& vertices = billboards->m_vertices;
    QVector<GLuint>& indices = billboards->m_indices;
    QVector3D& color = billboards->m_color;
    QVector<float>& rotations = billboards->m_rotations;

    QVector3D right = m_rightVector;
    QVector3D up = m_upVector;

    QVector3D ul = (0.5*up - 0.5*right)*scale;
    QVector3D ur = (0.5*up + 0.5*right)*scale;
    QVector3D dl = (-0.5*up - 0.5*right)*scale;
    QVector3D dr = (-0.5*up + 0.5*right)*scale;

    int numberOfVertices = positions.size()*4;
    vertices.resize(numberOfVertices);
    indices.resize(6*positions.size());

    QVector3D normalColor = color;

    for(auto i=0; i<positions.size(); i++) {
        QVector3D &position = positions[i];
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
        vertices[4*i + 0].position[2] += dl[2];
        vertices[4*i + 0].textureCoord= QVector2D(0,1);

        vertices[4*i + 1].position = position;
        vertices[4*i + 1].position[0] += dr[0]*cosTheta - dr[1]*sinTheta;
        vertices[4*i + 1].position[1] += dr[0]*sinTheta + dr[1]*cosTheta;
        vertices[4*i + 1].position[2] += dr[2];
        vertices[4*i + 1].textureCoord= QVector2D(1,1);

        vertices[4*i + 2].position = position;
        vertices[4*i + 2].position[0] += ur[0]*cosTheta - ur[1]*sinTheta;
        vertices[4*i + 2].position[1] += ur[0]*sinTheta + ur[1]*cosTheta;
        vertices[4*i + 2].position[2] += ur[2];
        vertices[4*i + 2].textureCoord= QVector2D(1,0);

        vertices[4*i + 3].position = position;
        vertices[4*i + 3].position[0] += ul[0]*cosTheta - ul[1]*sinTheta;
        vertices[4*i + 3].position[1] += ul[0]*sinTheta + ul[1]*cosTheta;
        vertices[4*i + 3].position[2] += ul[2];
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
    glFunctions()->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(BillboardVBOData), &vertices.front(), GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
}

void Billboards::setPositions(QVector<QVector3D> &positions)
{
    m_positions = positions;
}

void BillboardsRenderer::beforeLinkProgram() {
    // program().addShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/billboards/billboards.vsh");
    // program().addShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/billboards/billboards.fsh");
    program().addShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/billboards/billboards_lighted.vsh");
    program().addShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/billboards/billboards_lighted.fsh");
}

void BillboardsRenderer::render()
{
    if(m_vertexCount == 0) {
        return;
    }

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;
    program().setUniformValue("modelViewProjectionMatrix", modelViewProjectionMatrix);
    program().setUniformValue("shadowColor", m_shadowColor);

    // Tell OpenGL which VBOs to use
    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program().attributeLocation("a_position");
    program().enableAttributeArray(vertexLocation);
    glFunctions()->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(BillboardVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex color data
    int colorLocation = program().attributeLocation("a_color");
    program().enableAttributeArray(colorLocation);
    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(BillboardVBOData), (const void *)offset);

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program().attributeLocation("a_texcoord");
    program().enableAttributeArray(texcoordLocation);
    glFunctions()->glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(BillboardVBOData), (const void *)offset);

    // Draw cube geometry using indices from VBO 1
    m_texture->bind();
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);

    program().disableAttributeArray(vertexLocation);
    program().disableAttributeArray(colorLocation);
    program().disableAttributeArray(texcoordLocation);
}
