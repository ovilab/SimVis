#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "mysimulator.h"

#ifdef STATIC_BUILD
Q_IMPORT_PLUGIN(SimVisPlugin)
#endif

int main(int argc, char *argv[])
{
    qmlRegisterType<MySimulator>("MySimulator", 1, 0, "MySimulator");

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
#ifdef STATIC_BUILD
    engine.addImportPath(":/org.compphys.SimVis/imports");
#endif
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
