#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "cylindersimulator.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<CylinderSimulator>("CylinderSimulator", 1, 0, "CylinderSimulator");

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    qpm::init(app, engine);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
