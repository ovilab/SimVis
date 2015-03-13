#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "mysimulator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
