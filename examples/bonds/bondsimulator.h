#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>
#include <SimVis/CylinderData>

class BondWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    BondWorker();

private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();
    QVector<QVector3D> m_positions;
    QVector<CylinderVBOData> m_cylinders;
    float dt = 0.05;
    QElapsedTimer m_timer;
};

class BondSimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)
    Q_PROPERTY(CylinderData* cylinderData READ cylinderData CONSTANT)

public:
    BondSimulator(QNode *parent = 0);

    double dt() const;
    SphereData* sphereData()
    {
        return m_sphereData.data();
    }
    CylinderData* cylinderData()
    {
        return m_cylinderData.data();
    }

public slots:
    void setDt(double dt);

signals:
    void dtChanged(double dt);

protected:
    virtual SimulatorWorker *createWorker() override;

private:
    double m_dt = 0.05;
    QScopedPointer<SphereData> m_sphereData;
    QScopedPointer<CylinderData> m_cylinderData;

    QT3D_CLONEABLE(BondSimulator)
};

#endif // MYSIMULATOR_H
