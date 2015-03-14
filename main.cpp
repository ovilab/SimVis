#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
#include "billboards2d.h"
#include "points2d.h"
#include "visualizer.h"

int main(int argc, char *argv[])
{
    qmlRegisterInterface<Simulator>("Simulator");
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<Billboards2D>("MySimulator", 1, 0, "Billboards2D");
    qmlRegisterType<Points2D>("MySimulator", 1, 0, "Points2D");
    qmlRegisterType<Visualizer>("MySimulator", 1, 0, "Visualizer");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
