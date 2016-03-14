#include <QApplication>
#include <QQmlApplicationEngine>
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

    QQmlApplicationEngine engine;
    qpm::init(app, engine);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
