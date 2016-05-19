#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>
#include <SimVis/BondData>

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
    QVector<BondVBOData> m_bonds;
    float dt = 0.05;
    QElapsedTimer m_timer;
};

class BondSimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)
    Q_PROPERTY(BondData* bondData READ bondData CONSTANT)

public:
    BondSimulator(QNode *parent = 0);

    double dt() const;
    SphereData* sphereData()
    {
        return m_sphereData.data();
    }
    BondData* bondData()
    {
        return m_bondData.data();
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
    QScopedPointer<BondData> m_bondData;

    QT3D_CLONEABLE(BondSimulator)
};

#endif // MYSIMULATOR_H
