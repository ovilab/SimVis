#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"
#include "billboards2d.h"
#include "visualizer.h"

int main(int argc, char *argv[])
{
    qRegisterMetaType<MySimulator*>();
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    qmlRegisterType<Billboards2D>("MySimulator", 1, 0, "Billboards2D");
    qmlRegisterType<Visualizer>("MySimulator", 1, 0, "Visualizer");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
