#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include "../../core/renderable.h"

class MarchingCubesRenderer : public RenderableRenderer
{
public:
    MarchingCubesRenderer();

private:
    virtual void synchronize(Renderable *) override;
    virtual void render() override;

    void uploadVBOs(Billboards* billboards);

    virtual void beforeLinkProgram() override;
    bool m_isInitialized = false;
};

class MarchingCubes : public Renderable
{
public:
    MarchingCubes();
    ~MarchingCubes();

    // Renderable interface
public:
    RenderableRenderer *createRenderer();
};

#endif // MARCHINGCUBES_H
