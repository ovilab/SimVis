#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QQuickView>

#include "neuronreader.h"

#include <vendor.h>

int main(int argc, char *argv[])
{
    qmlRegisterType<NeuronReader>("NeuroML", 1, 0, "NeuronReader");
    QApplication app(argc, argv);

    QQuickView view;
    qpm::init(app, *view.engine());
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    view.setFormat(format);
    view.setSource(QUrl("qrc:/main_neuron.qml"));
    view.show();

    return app.exec();
}
