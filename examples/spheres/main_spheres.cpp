#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "spheressimulator.h"
#include "pointgeometry.h"
#include "spheredata.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<PointGeometry>("MySimulator", 1, 0, "PointGeometry");
    qmlRegisterType<SphereData>("MySimulator", 1, 0, "SphereData");
    QApplication app(argc, argv);

    QQuickView view;
    qpm::init(app, *view.engine());
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
//    view.setFormat(format);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
