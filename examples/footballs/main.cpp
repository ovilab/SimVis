#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "mysimulator.h"

int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath("/home/svenni/Dropbox/projects/programming/compphys/build-SimVis-Desktop_Qt_5_4_0_GCC_64bit-Debug/src");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
