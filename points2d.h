//#pragma once
//#ifndef POINTS2D_H
//#define POINTS2D_H

//#include "renderable.h"
//#include <QtGui/QOpenGLShaderProgram>
//#include <QOpenGLFunctions>
//#include <QOpenGLTexture>
//#include <vector>
//using std::vector;

//class Simulator;

//class Points2D;
//class Points2DRenderer : public RenderableRenderer
//{
//    virtual void synchronize(Renderable *);
//    virtual void render();

//    void uploadVBOs(Points2D* billboards);
//    void createShaderProgram();
//    bool m_isInitialized = false;

//    vector<GLuint> m_vboIds;
//    QOpenGLTexture *m_texture = 0;
//    bool m_isTextureUploaded = false;
//    QOpenGLShaderProgram* m_program;
//    int m_vertexCount;
//    int m_indexCount;
//};

//class Billboards2D : public Renderable
//{
//    Q_OBJECT
//    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
//    Q_PROPERTY(QString texture READ texture WRITE setTexture NOTIFY textureChanged)
//public:
//    Billboards2D();
//    ~Billboards2D();
//    virtual void initialize();
//    virtual void copyDataFunction();
//    void setPositions(std::vector<QVector2D> &positions);
//    std::vector<QVector2D> &positions();
//    std::vector<float> &rotations();
//    std::function<void(Billboards2D *renderableObject)> myCopyData;

//    float scale() const;
//    void setScale(float scale);

//    QVector3D color() const;
//    void setColor(const QColor &color);

//    virtual RenderableRenderer* createRenderer();

//    QString texture() const
//    {
//        return m_texture;
//    }

//public slots:
//    void setTexture(QString arg)
//    {
//        if (m_texture == arg)
//            return;

//        m_texture = arg;
//        emit textureChanged(arg);
//    }

//signals:
//    void scaleChanged(bool arg);

//    void textureChanged(QString arg);

//private:
//    std::vector<Billboard2DVBOData> m_vertices;
//    std::vector<GLuint> m_indices;
//    QVector3D m_color;

//    std::vector<QVector2D> m_positions;
//    std::vector<float> m_rotations;
//    float m_scale = 1.0;

//    QVector3D vectorFromColor(const QColor &color);

//    friend class Billboards2DRenderer;
//    QString m_texture = "NO TEXTURE CHOSEN";
//};


//#endif // POINTS2D_H
