#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "simulator.h"
#include <QDebug>
#include <QVector2D>

class MyWorker : public SimulatorWorker
{
    Q_OBJECT
public:
    MyWorker();
    void synchronizeSimulator(Simulator *simulator);
    void synchronizeRenderer(Renderable *renderableObject);

public slots:
    void work();

private:
    float m_springConstant = 1.0;
    float m_mass = 1.0;
    float m_dt = 0.01;
    std::vector<QVector2D> m_positions;
    std::vector<QVector2D> m_velocities;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(float springConstant READ springConstant WRITE setSpringConstant NOTIFY springConstantChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float dt READ dt WRITE setDt NOTIFY dtChanged)
public:
    explicit MySimulator();
    ~MySimulator();

    float springConstant() const
    {
        return m_springConstant;
    }

    float mass() const
    {
        return m_mass;
    }

    float dt() const
    {
        return m_dt;
    }

public slots:
    void setSpringConstant(float arg)
    {
        if (m_springConstant == arg)
            return;

        m_springConstant = arg;
        emit springConstantChanged(arg);
    }

    void setMass(float arg)
    {
        if (m_mass == arg)
            return;

        m_mass = arg;
        emit massChanged(arg);
    }

    void setDt(float arg)
    {
        if (m_dt == arg)
            return;

        m_dt = arg;
        emit dtChanged(arg);
    }

signals:
    void springConstantChanged(float arg);

    void massChanged(float arg);

    void dtChanged(float arg);

protected:
    SimulatorWorker *createWorker();
private:
    float m_springConstant = 1.0;
    float m_mass = 1.0;
    float m_dt = 0.01;
};

#endif // MYSIMULATOR_H
