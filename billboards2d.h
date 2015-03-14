#pragma once
#ifndef BILLBOARDS2D_H
#define BILLBOARDS2D_H

#include "renderable.h"
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <vector>
using std::vector;

class Simulator;

struct Billboard2DData
{
    QVector2D position;
    QVector3D color;
    QVector2D textureCoord;
};

class Billboards2D;
class Billboards2DRenderer : public RenderableRenderer
{
    virtual void synchronize(Renderable *);
    virtual void render(QMatrix4x4 &modelViewMatrix, QMatrix4x4 &projectionMatrix);

    void uploadVBOs(Billboards2D* billboards);

    void createShaderProgram();
    void uploadTexture(QString filename);

    bool m_isInitialized = false;

    vector<GLuint> m_vboIds;
    QOpenGLTexture *m_texture = 0;
    bool m_isTextureUploaded = false;
    QString m_textureFilename = "football.png";
    QOpenGLShaderProgram* m_program;
    int m_vertexCount;
    int m_indexCount;
};

class Billboards2D : public Renderable
{
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    Billboards2D();
    ~Billboards2D();
    virtual void initialize();
    virtual void copyDataFunction();
    void setPositions(std::vector<QVector2D> &positions);
    std::vector<QVector2D> &positions();
    std::vector<float> &rotations();
    std::function<void(Billboards2D *renderableObject)> myCopyData;

    float scale() const;
    void setScale(float scale);

    QVector3D color() const;
    void setColor(const QColor &color);

    virtual RenderableRenderer* createRenderer();

signals:
    void scaleChanged(bool arg);

private:
    std::vector<Billboard2DData> m_vertices;
    std::vector<GLuint> m_indices;
    QVector3D m_color;

    std::vector<QVector2D> m_positions;
    std::vector<float> m_rotations;
    float m_scale = 1.0;

    QVector3D vectorFromColor(const QColor &color);

    friend class Billboards2DRenderer;
};


#endif // BILLBOARDS2D_H
