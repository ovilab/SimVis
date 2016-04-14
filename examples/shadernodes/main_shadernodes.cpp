#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "spheregeometrynew.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<Qt3DRender::SphereGeometryNew>("SimVis", 1, 0, "SphereGeometryNew");

    QQuickView view;
    qpm::init(app, *view.engine());
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    view.setFormat(format);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
