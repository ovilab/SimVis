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
                // QVector3D center = QVector3D(5,5,5);
                // QVector3D newVec = vec - center;
                if(vec.x() < -3 || vec.x() > 3) value = 5.0;
                if(vec.y() < -3 || vec.y() > 3) value = 5.0;
                if(vec.z() < -3 || vec.z() > 3) value = 5.0;


                // return vec.lengthSquared();
                return value;
            });

            m_didSetScalarFieldEvaluator = true;
        }
    }
}
