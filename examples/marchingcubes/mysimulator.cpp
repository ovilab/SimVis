#include "mysimulator.h"
#include <SimVis/MarchingCubes>
#include <cmath>
MyWorker::MyWorker() {

}

SimulatorWorker *MySimulator::createWorker() {
    return new MyWorker();
}

void MyWorker::synchronizeSimulator(Simulator *simulator) {

}

void MyWorker::work() {

}

void MyWorker::synchronizeRenderer(Renderable *renderableObject)
{
    MarchingCubes* marchingCubes = qobject_cast<MarchingCubes*>(renderableObject);
    if(marchingCubes) {
        if(!m_didSetScalarFieldEvaluator) {
            marchingCubes->setScalarFieldEvaluator([](const QVector3D point) {
                // return sin(point.x()*point.y()) + point.z()*cos(point.z());
                return point.length();
            });

            marchingCubes->setColorEvaluator([](const QVector3D point) {
                float min = -5;
                float max = 5;

                QVector3D color = point;
                color[0] -= min;
                color[1] -= min;
                color[2] -= min;

                color[0] /= max - min;
                color[1] /= max - min;
                color[2] /= max - min;
                return color;
            });

            m_didSetScalarFieldEvaluator = true;
        }
    }
}
