#ifndef CYLINDERSIMULATOR_H
#define CYLINDERSIMULATOR_H

#include <SimVis/Simulator>
#include <SimVis/CylinderData>
#include <SimVis/SphereData>

#include <QVector>
#include <QVector3D>

class CylinderWorker : public SimulatorWorker
{
    Q_OBJECT
private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void synchronizeRenderer(Renderable *renderableObject);
    virtual void work();
    QVector<CylinderVBOData> m_cylinders;
public:
    CylinderWorker();
};

class CylinderSimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(CylinderData* cylinderData READ cylinderData CONSTANT)
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)
public:
    CylinderSimulator(QNode *parent = nullptr);

    CylinderData *cylinderData() {
        return m_cylinderData.data();
    }

    SphereData *sphereData() {
        return m_sphereData.data();
    }

protected:
    virtual SimulatorWorker *createWorker();

private:
    QScopedPointer<CylinderData> m_cylinderData;
    QScopedPointer<SphereData> m_sphereData;

    QT3D_CLONEABLE(CylinderSimulator)
};

#endif // CYLINDERSIMULATOR_H
