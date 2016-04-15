#include "mysimulator.h"
#include <SimVis/TriangleCollection>
#include <SimVis/Lines>
#include <QDebug>
MySimulator::MySimulator()
{

}

SimulatorWorker *MySimulator::createWorker()
{
    return new MyWorker();
}

void MyWorker::synchronizeSimulator(Simulator *simulator)
{
    Q_UNUSED(simulator)
}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    TriangleCollection* triangleCollection = qobject_cast<TriangleCollection*>(renderableObject);
    if(triangleCollection) {
        triangleCollection->data.resize(3);
        triangleCollection->data[0].vertex = QVector3D(-1, -1, 0);
        triangleCollection->data[1].vertex = QVector3D(1, 1, 0);
        triangleCollection->data[2].vertex = QVector3D(1, -1, 0);
        triangleCollection->dirty = true;
        return;
    }

    Lines *lines = qobject_cast<Lines*>(renderableObject);
    if(lines) {
        lines->data.clear();
        lines->data.append(QVector3D(-1, -1, -1));
        lines->data.append(QVector3D(-1, -1, 1));

        lines->data.append(QVector3D(1, 1, -1));
        lines->data.append(QVector3D(1, 1, 1));

        lines->data.append(QVector3D(1, -1, -1));
        lines->data.append(QVector3D(1, -1, 1));

        lines->data.append(QVector3D(-1, 1, -1));
        lines->data.append(QVector3D(-1, 1, 1));

        lines->data.append(QVector3D(-1, -1, -1));
        lines->data.append(QVector3D(1, -1, -1));
        lines->data.append(QVector3D(-1, -1, -1));
        lines->data.append(QVector3D(-1, 1, -1));

        lines->data.append(QVector3D(1, 1, -1));
        lines->data.append(QVector3D(1, -1, -1));
        lines->data.append(QVector3D(1, 1, -1));
        lines->data.append(QVector3D(-1, 1, -1));

        lines->data.append(QVector3D(1, 1, 1));
        lines->data.append(QVector3D(1, -1, 1));
        lines->data.append(QVector3D(1, 1, 1));
        lines->data.append(QVector3D(-1, 1, 1));

        lines->data.append(QVector3D(-1, -1, 1));
        lines->data.append(QVector3D(1, -1, 1));
        lines->data.append(QVector3D(-1, -1, 1));
        lines->data.append(QVector3D(-1, 1, 1));
    }
}

void MyWorker::work()
{

}

MyWorker::MyWorker() {

}
