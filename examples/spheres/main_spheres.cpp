#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include "spheressimulator.h"
#include "instancebuffer.h"
#include "pointgeometry.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<InstanceBuffer>("MySimulator", 1, 0, "InstanceBuffer");
    qmlRegisterType<PointGeometry>("MySimulator", 1, 0, "PointGeometry");
    QApplication app(argc, argv);

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
