#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQml>

#include <vendor.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQuickView view;
    qpm::init(app, *view.engine());
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
