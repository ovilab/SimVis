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
    void reset();
    float m_springConstant = 1.0;
    float m_mass = 1.0;
    float m_dt = 0.01;
    int   m_numberOfBalls = 100;
    std::vector<QVector2D> m_positions;
    std::vector<QVector2D> m_velocities;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(float springConstant READ springConstant WRITE setSpringConstant NOTIFY springConstantChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(int numberOfBalls READ numberOfBalls WRITE setNumberOfBalls NOTIFY numberOfBallsChanged)
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

    int numberOfBalls() const
    {
        return m_numberOfBalls;
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

    void reset();

    void setNumberOfBalls(int arg)
    {
        if (m_numberOfBalls == arg)
            return;

        m_numberOfBalls = arg;
        emit numberOfBallsChanged(arg);
    }

signals:
    void springConstantChanged(float arg);
    void massChanged(float arg);
    void dtChanged(float arg);
    void numberOfBallsChanged(int arg);

protected:
    SimulatorWorker *createWorker();
private:
    float m_springConstant = 1.0;
    float m_mass = 1.0;
    float m_dt = 0.01;
    int m_numberOfBalls = 100;
    bool m_willReset = true;
    friend class MyWorker;
};

#endif // MYSIMULATOR_H
