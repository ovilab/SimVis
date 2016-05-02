#ifndef MARCHINGCUBESTABLETEXTURE_H
#define MARCHINGCUBESTABLETEXTURE_H
#include <QAbstractTextureImage>
#include <QTexImageData>
#include <Qt3DRender/QTexture>
#include <QImage>
#include <QColor>

class MCDataFunctor : public Qt3DRender::QTextureImageDataGenerator {
    // QAbstractFunctor interface
public:
    MCDataFunctor() : Qt3DRender::QTextureImageDataGenerator() { }
    ~MCDataFunctor() {}
    virtual Qt3DRender::QTexImageDataPtr operator ()() override;
    virtual bool operator ==(const Qt3DRender::QTextureImageDataGenerator &other) const override;
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
    virtual Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const override;
};

#endif // MARCHINGCUBESTABLETEXTURE_H
