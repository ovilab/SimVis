#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <QVector>
#include <QVector3D>
#include <QElapsedTimer>
#include <Qt3DRender/QBuffer>
#include <SimVis/SphereData>

class SpheresWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    SpheresWorker();

private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void work();
    QVector<QVector3D> m_positions;
    QVector<QVector3D> m_velocities;
    float dt = 0.05;
    QElapsedTimer m_timer;
};

class SpheresSimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(SphereData* sphereData READ sphereData CONSTANT)

public:
    SpheresSimulator(QNode *parent = 0);
    double dt() const;
    SphereData* sphereData();

public slots:
    void setDt(double dt);

signals:
    void dtChanged(double dt);

protected:
    virtual SimulatorWorker *createWorker() override;

private:
    double m_dt = 0.05;
    QScopedPointer<SphereData> m_sphereData;
};

#endif // MYSIMULATOR_H
