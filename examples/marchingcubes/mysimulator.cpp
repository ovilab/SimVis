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
                float n = 4.0;
                return sin(n*M_PI*0.5*point.x()) + sin(n*M_PI*0.5*point.y()) + sin(n*M_PI*0.5*point.z());
            });

            marchingCubes->setColorEvaluator([](const QVector3D point) {
                float min = -M_PI;
                float max = M_PI;

                QVector4D color = QVector4D(point, 1.0);
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
