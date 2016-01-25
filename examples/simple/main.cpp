#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQuickView>
#include <QtGui>
#include <QtQuick>
#include <QDebug>
#include <iostream>

#include "simplesimulator.h"
//#include "simvisplugin.h"
#include "core/simulator.h"
#include "core/../renderables/billboards/billboards.h"
#include "core/../renderables/points/points.h"
#include "core/../renderables/spheres/spheres.h"
#include "core/../renderables/cylinders/cylinders.h"
#include "core/../renderables/lines/lines.h"
#include "core/../renderables/marchingcubes/marchingcubes.h"
#include "core/../renderables/skybox/skybox.h"
#include "core/visualizer.h"
#include "core/camera.h"
#include "core/navigator.h"
#include "core/../navigators/trackballnavigator.h"
#include "core/../shadereffects/simplexbump.h"
#include "core/../shadereffects/light.h"
#include "core/../shadereffects/simplextexture.h"
#include "core/../shadereffects/skyboxreflection.h"
#include "core/../renderables/trianglecollection/trianglecollection.h"
#include "core/../shadereffects/periodiccopies.h"
#include "core/../shadereffects/slice.h"

//Q_IMPORT_PLUGIN(SimVisPlugin)

QQuickView *view;

void app_init(int argc, char **argv)
{
    qmlRegisterUncreatableType<Simulator>("SimVis", 1, 0, "Simulator",
                                          "Cannot create abstract type Simulator. This must be subclassed.");
    qmlRegisterType<Billboards>("SimVis", 1, 0, "Billboards");
    qmlRegisterType<Spheres>("SimVis", 1, 0, "Spheres");
    qmlRegisterType<MarchingCubes>("SimVis", 1, 0, "MarchingCubes");
    qmlRegisterType<Points>("SimVis", 1, 0, "Points");
    qmlRegisterType<Visualizer>("SimVis", 1, 0, "Visualizer");
    qmlRegisterType<Camera>("SimVis", 1, 0, "Camera");
    qmlRegisterType<Navigator>("SimVis", 1, 0, "Navigator");
    qmlRegisterType<TrackballNavigator>("SimVis", 1, 0, "TrackballNavigator");
    qmlRegisterType<SimplexBump>("SimVis", 1, 0, "SimplexBump");
    qmlRegisterType<SimplexTexture>("SimVis", 1, 0, "SimplexTexture");
    qmlRegisterType<Light>("SimVis", 1, 0, "Light");
    qmlRegisterType<SkyBox>("SimVis", 1, 0, "SkyBox");
    qmlRegisterType<SkyBoxReflection>("SimVis", 1, 0, "SkyBoxReflection");
    qmlRegisterType<TriangleCollection>("SimVis", 1, 0, "TriangleCollection");
    qmlRegisterType<PeriodicCopies>("SimVis", 1, 0, "PeriodicCopies");
    qmlRegisterType<Slice>("SimVis", 1, 0, "Slice");
    qmlRegisterType<Cylinders>("SimVis", 1, 0, "Cylinders");
    qmlRegisterType<Lines>("SimVis", 1, 0, "Lines");
    qmlRegisterType<SimpleSimulator>("SimpleSimulator", 1, 0, "SimpleSimulator");
    qDebug() << "Woooot!";
    std::cerr << "Woot" << std::endl;
    view = new QQuickView();
    view->setSource(QUrl("qrc:///main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();
    qDebug() << "View should be visible";
}

void app_exit()
{
    delete view;
}

#ifdef Q_OS_NACL
Q_GUI_MAIN(app_init, app_exit);
#else

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app_init(argc, argv);

    int retVal = app.exec();
    app_exit();
    return retVal;
}
#endif
