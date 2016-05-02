#ifndef MARCHINGCUBESTABLETEXTURE_H
#define MARCHINGCUBESTABLETEXTURE_H
#include <QAbstractTextureImage>
#include <QTexture2D>
#include <QTexImageData>
#include <QImage>
#include <QColor>

class MCDataFunctor : public Qt3DRender::QTextureDataFunctor {
    // QAbstractFunctor interface
public:
    MCDataFunctor() : Qt3DRender::QTextureDataFunctor() { }
    ~MCDataFunctor() {}
    virtual Qt3DRender::QTexImageDataPtr operator ()() override;
    virtual bool operator ==(const Qt3DRender::QTextureDataFunctor &other) const override;
    QT3D_FUNCTOR(MCDataFunctor)
};

class MarchingCubesTableTexture : public Qt3DRender::QAbstractTextureImage
{
    Q_OBJECT
public:
    MarchingCubesTableTexture() {}
    ~MarchingCubesTableTexture() {}
private:
    QT3D_CLONEABLE(MarchingCubesTableTexture)
    // QAbstractTextureImage interface
public:
    virtual Qt3DRender::QTextureDataFunctorPtr dataFunctor() const override;
};

#endif // MARCHINGCUBESTABLETEXTURE_H
