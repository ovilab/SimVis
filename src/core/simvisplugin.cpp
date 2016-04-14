#include "simvisplugin.h"
#include "simulator.h"
#include "../renderables/billboards/billboards.h"
#include "../renderables/points/points.h"
#include "../renderables/lines/lines.h"
#include "../generators/marchingcubes/marchingcubes.h"
#include "../renderables/skybox/skybox.h"
#include "visualizer.h"
#include "navigator.h"
#include "navigators/trackballnavigator.h"
#include "navigators/flymodenavigator.h"
#include "../renderables/marchingcubes/marchingcubesgs.h"
#include "../shadereffects/simplexbump.h"
#include "../shadereffects/light.h"
#include "../shadereffects/simplextexture.h"
#include "../shadereffects/skyboxreflection.h"
#include "../renderables/trianglecollection/trianglecollection.h"
#include "../renderables/noiseviewer/noiseviewer.h"
#include "../shadereffects/periodiccopies.h"
#include "../shadereffects/slice.h"
#include "../shadernodes/shadernode.h"
#include "../shadernodes/shaderbuilder.h"
#include "../shadernodes/shaderoutput.h"
#include "../shadernodes/shadergroup.h"
#include "../shadernodes/shaderbuilderbinding.h"
#include "../render/geometry/pointgeometry.h"
#include "../render/geometry/spheredata.h"
#include "../render/geometry/cylinderdata.h"
#include "../render/geometry/uniformgridgeometry.h"
#include "../utils/marchingcubestabletexture.h"

#include <qqml.h>
#include <QQmlEngine>


void SimVisPlugin::registerTypes(const char *uri)
{
    Q_INIT_RESOURCE(simvis_shaders);
    // @uri SimVis
    Q_ASSERT(uri == QLatin1String("SimVis"));
    qmlRegisterUncreatableType<Simulator>("SimVis", 1, 0, "Simulator",
                                          "Cannot create abstract type Simulator. This must be subclassed.");
    qmlRegisterType<Billboards>("SimVis", 1, 0, "Billboards");
    qmlRegisterType<MarchingCubes>("SimVis", 1, 0, "MarchingCubes");
    qmlRegisterType<Points>("SimVis", 1, 0, "Points");
    qmlRegisterType<Navigator>("SimVis", 1, 0, "Navigator");
    qmlRegisterType<TrackballNavigator>("SimVis", 1, 0, "TrackballNavigator");
    qmlRegisterType<FlyModeNavigator>("SimVis", 1, 0, "FlyModeNavigator");
    qmlRegisterType<SimplexBump>("SimVis", 1, 0, "SimplexBump");
    qmlRegisterType<SimplexTexture>("SimVis", 1, 0, "SimplexTexture");
    qmlRegisterType<Light>("SimVis", 1, 0, "RenderableLight");
    qmlRegisterType<SkyBox>("SimVis", 1, 0, "SkyBox");
    qmlRegisterType<SkyBoxReflection>("SimVis", 1, 0, "SkyBoxReflection");
    qmlRegisterType<NoiseViewer>("SimVis", 1, 0, "NoiseViewer");
    qmlRegisterType<PeriodicCopies>("SimVis", 1, 0, "PeriodicCopies");
    qmlRegisterType<Slice>("SimVis", 1, 0, "Slice");
    qmlRegisterType<Lines>("SimVis", 1, 0, "Lines");
    qmlRegisterType<ShaderNode>("SimVis", 1, 0, "ShaderNode");
    qmlRegisterType<ShaderBuilder>("SimVis", 1, 0, "ShaderBuilder");
    qmlRegisterType<ShaderOutput>("SimVis", 1, 0, "ShaderOutput");
    qmlRegisterType<ShaderGroup>("SimVis", 1, 0, "ShaderGroup");
    qmlRegisterType<ShaderBuilderBinding>("SimVis", 1, 0, "ShaderBuilderBinding");
    qmlRegisterType<MarchingCubesGS>("SimVis", 1, 0, "AbstractMarchingCubes");
    qmlRegisterType<PointGeometry>("SimVis", 1, 0, "PointGeometry");
    qmlRegisterType<UniformGridGeometry>("SimVis", 1, 0, "UniformGridGeometry");
    qmlRegisterType<SphereData>("SimVis", 1, 0, "SphereData");
    qmlRegisterType<CylinderData>("SimVis", 1, 0, "CylinderData");
    qmlRegisterType<TriangleCollection>("SimVis", 1, 0, "AbstractTriangleCollection");
    qmlRegisterType<MarchingCubesTableTexture>("SimVis", 1, 0, "MarchingCubesTableTexture");
}

void SimVisPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}


