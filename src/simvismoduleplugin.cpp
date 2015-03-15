#include "simvismoduleplugin.h"

#include "simulator.h"
#include "billboards2d.h"
#include "points2d.h"
#include "visualizer.h"

#include <QtQml>

void SimVisModulePlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("SimVis"));
    qmlRegisterUncreatableType<Simulator>(uri, 1, 0, "Simulator", "nope");
    qmlRegisterType<Billboards2D>(uri, 1, 0, "Billboards2D");
    qmlRegisterType<Points2D>(uri, 1, 0, "Points2D");
    qmlRegisterType<Visualizer>(uri, 1, 0, "Visualizer");
}
