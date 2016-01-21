#ifndef LINES_H
#define LINES_H
#include "../../core/renderable.h"

class LinesRenderer : public RenderableRenderer
{
    Q_OBJECT
public:
    LinesRenderer();
private:
    virtual void synchronize(Renderable *renderable) override;
    virtual void render() override;

    void uploadVBO(class Lines* points);
    void beforeLinkProgram() override;
    int m_vertexCount = 0;
    bool m_isInitialized = false;
};

class Lines : public Renderable
{
    Q_OBJECT
public:
    Lines();
    QVector<QVector3D> data;

    // Renderable interface
public:
    virtual LinesRenderer *createRenderer() override;
};

#endif // LINES_H
