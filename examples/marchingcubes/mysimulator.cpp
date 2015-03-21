#include "mysimulator.h"
#include <SimVis/MarchingCubes>
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
            marchingCubes->setScalarFieldEvaluator([&](const QVector3D vec) {
                float value = 0;
                if(vec.x() < -1 || vec.x() > 1) value = 5.0;
                if(vec.y() < -1 || vec.y() > 1) value = 5.0;
                if(vec.z() < -1 || vec.z() > 1) value = 5.0;

                // return vec.lengthSquared();
                return value;
            });

            m_didSetScalarFieldEvaluator = true;
        }
    }
}
