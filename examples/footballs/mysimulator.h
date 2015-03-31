#ifndef MYSIMULATOR_H
#define MYSIMULATOR_H
#include <SimVis/Simulator>
#include <SimVis/QuickWorker>
#include <QDebug>
#include <QVector3D>

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
    float m_dt = 0.001;
    int   m_ballCount = 100;
    QVector<QVector3D> m_velocities;
};

class MySimulator : public Simulator
{
    Q_OBJECT
    Q_PROPERTY(float springConstant READ springConstant WRITE setSpringConstant NOTIFY springConstantChanged)
    Q_PROPERTY(float mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(float dt READ dt WRITE setDt NOTIFY dtChanged)
    Q_PROPERTY(int ballCount READ ballCount WRITE setBallCount NOTIFY ballCountChanged)
public:
    explicit MySimulator();
    ~MySimulator();

    float springConstant() const;
    float mass() const;
    float dt() const;
    int ballCount() const;

public slots:
    void setSpringConstant(float arg);
    void setMass(float arg);
    void setDt(float arg);
    void reset();
    void setBallCount(int arg);

signals:
    void springConstantChanged(float arg);
    void massChanged(float arg);
    void dtChanged(float arg);
    void ballCountChanged(int arg);

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
