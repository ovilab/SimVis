#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/TriangleCollection>

#include <QVector>
#include <QVector3D>

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();
private:
    // SimulatorWorker interface
    virtual void synchronizeSimulator(Simulator *simulator);
    virtual void synchronizeRenderer(Renderable *renderableObject);
    virtual void work();
    QVector<QVector3D> m_positions;
    QVector<QVector3D> m_velocities;
    float dt = 0.05;
    QElapsedTimer m_timer;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(double dt READ dt WRITE setDt NOTIFY dtChanged)

public:
    MySimulator();
    double dt() const;

public slots:
    void setDt(double dt);

signals:
    void dtChanged(double dt);

protected:
    virtual SimulatorWorker *createWorker();

private:
    double m_dt = 0.05;

};

#endif // MYSIMULATOR_H
