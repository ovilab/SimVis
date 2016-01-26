#ifndef FLYMODENAVIGATOR_H
#define FLYMODENAVIGATOR_H
#include <QTimer>
#include "../core/navigator.h"
class FlyModeNavigator : public Navigator
{
    Q_OBJECT
    Q_PROPERTY(float maxSpeed READ maxSpeed WRITE setMaxSpeed NOTIFY maxSpeedChanged)
private:
    QTimer m_timer;
    float m_maxSpeed = 1.0;

public:
    explicit FlyModeNavigator(QQuickItem *parent = 0);
    float maxSpeed() const;

public slots:
    void tick();
    void setMaxSpeed(float maxSpeed);

signals:
    void maxForwardSpeedChanged(float maxForwardSpeed);
    void maxSpeedChanged(float maxSpeed);

protected:
    bool m_shiftPressed = false;
    float m_forwardSpeed = 0;
    float m_rightSpeed = 0;
    bool m_forwardIsPressed = false;
    bool m_backwardIsPressed = false;
    bool m_rightIsPressed = false;
    bool m_leftIsPressed = false;
    QPointF lastMoveEvent;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
    QPoint absolutePosition(QPoint p);
    QWindow *getCurrentWindow();
};

#endif // FLYMODENAVIGATOR_H
