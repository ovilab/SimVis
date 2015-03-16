#include "simvisplugin.h"
#include "simulator.h"
#include "../renderables/billboards.h"
#include "../renderables/points.h"
#include "visualizer.h"
#include "camera.h"

#include <qqml.h>

void SimVisPlugin::registerTypes(const char *uri)
{
    // @uri SimVis
    Q_ASSERT(uri == QLatin1String("SimVis"));
    qmlRegisterUncreatableType<Simulator>("SimVis", 1, 0, "Simulator",
                                          "Cannot create abstract type Simulator. This must be subclassed.");
    qmlRegisterType<Billboards>("SimVis", 1, 0, "Billboards");
    qmlRegisterType<Points>("SimVis", 1, 0, "Points");
    qmlRegisterType<Visualizer>("SimVis", 1, 0, "Visualizer");
    qmlRegisterType<Camera>("SimVis", 1, 0, "Camera");
}


