#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include "simulator.h"
#include "quickworker.h"
#include <QDebug>
#include <QVector2D>

class MyWorker : public QuickWorker
{
    Q_OBJECT
public:
    MyWorker();

private:
    virtual void synchronizeSimulator(Simulator *simulator) override;
    virtual void work() override;
    void reset();
    float m_springConstant = 1.0;
    float m_mass = 1.0;
    float m_dt = 0.01;
    int   m_numberOfBalls = 100;
    QVector<QVector2D> m_velocities;
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

    float springConstant() const;
    float mass() const;
    float dt() const;
    int numberOfBalls() const;

public slots:
    void setSpringConstant(float arg);
    void setMass(float arg);
    void setDt(float arg);
    void reset();
    void setNumberOfBalls(int arg);

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
