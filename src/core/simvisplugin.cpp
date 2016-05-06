#include "simvisplugin.h"
#include "simulator.h"
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
    qmlRegisterType<ShaderNode>("SimVis", 1, 0, "ShaderNode");
    qmlRegisterType<ShaderBuilder>("SimVis", 1, 0, "ShaderBuilder");
    qmlRegisterType<ShaderOutput>("SimVis", 1, 0, "ShaderOutput");
    qmlRegisterType<ShaderGroup>("SimVis", 1, 0, "ShaderGroup");
    qmlRegisterType<ShaderBuilderBinding>("SimVis", 1, 0, "ShaderBuilderBinding");
    qmlRegisterType<PointGeometry>("SimVis", 1, 0, "PointGeometry");
    qmlRegisterType<UniformGridGeometry>("SimVis", 1, 0, "UniformGridGeometry");
    qmlRegisterType<SphereData>("SimVis", 1, 0, "SphereData");
    qmlRegisterType<CylinderData>("SimVis", 1, 0, "CylinderData");
    qmlRegisterType<MarchingCubesTableTexture>("SimVis", 1, 0, "MarchingCubesTableTexture");
}

void SimVisPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(engine);
    Q_UNUSED(uri);
}


