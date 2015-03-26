#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "perlinnoise.h"

#include <SimVis/Simulator>
#include <SimVis/QuickWorker>
#include <QVector3D>

class MyWorker;
class MySimulator : public Simulator
{
    Q_OBJECT
    Q_ENUMS(Geometry)
    Q_PROPERTY(Geometry geometry READ geometry WRITE setGeometry NOTIFY geometryChanged)
    Q_PROPERTY(bool continuousScalarField READ continuousScalarField WRITE setContinuousScalarField NOTIFY continuousScalarFieldChanged)
public:
    enum Geometry
    {
        SPHERE,
        SINUS,
        CUBE,
        PERLIN
    };

    Geometry geometry() const;

    bool continuousScalarField() const;

public slots:
    void setGeometry(Geometry arg);
    void setContinuousScalarField(bool arg);

signals:
    void geometryChanged(Geometry arg);
    void continuousScalarFieldChanged(bool arg);

protected:
    SimulatorWorker *createWorker();

private:
    Geometry m_geometry = SINUS;
    bool m_willSetScalarField = true;
    bool m_continuousScalarField = false;
    friend class MyWorker;
};

class MyWorker : public QuickWorker
{
    Q_OBJECT
public:

    MyWorker();

public slots:

signals:

private:
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void work() override;
    virtual void synchronizeRenderer(Renderable *renderableObject) override;
    bool m_willSetScalarField = true;
    bool m_continuousScalarField = false;
    PerlinNoise m_perlin;
    MySimulator::Geometry m_geometry = MySimulator::SINUS;
};

#endif // MYSIMULATOR_H
