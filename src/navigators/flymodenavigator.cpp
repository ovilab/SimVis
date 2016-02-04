#include "flymodenavigator.h"
#include "../core/camera.h"
#include <QCursor>
#include <QWindow>
#include <QApplication>
#include <cmath>
FlyModeNavigator::FlyModeNavigator(QQuickItem *parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(false);
    setFocus(false);
    connect(&m_timer, &QTimer::timeout, this, &FlyModeNavigator::tick);
    m_timer.setInterval(16);
    m_timer.start();
}

float FlyModeNavigator::maxSpeed() const
{
    return m_maxSpeed;
}

void FlyModeNavigator::tick()
{
    if(!m_camera) {
        return;
    }

    float dt = 0.1;
    if(m_shiftPressed) {
        dt = 0.5;
        m_camera->setViewCenter(m_camera->viewCenter() + m_forwardSpeed*dt*m_camera->viewVector().normalized());
        m_camera->setPosition(m_camera->position() + m_forwardSpeed*dt*m_camera->viewVector().normalized());
        m_camera->setViewCenter(m_camera->viewCenter() + m_rightSpeed*dt*m_camera->rightVector());
        m_camera->setPosition(m_camera->position() + m_rightSpeed*dt*m_camera->rightVector());
        m_camera->roll(3*m_rollSpeed*dt);
    } else {
        m_camera->setViewCenter(m_camera->viewCenter() + m_forwardSpeed*dt*m_camera->viewVector().normalized());
        m_camera->setPosition(m_camera->position() + m_forwardSpeed*dt*m_camera->viewVector().normalized());
        m_camera->setViewCenter(m_camera->viewCenter() + m_rightSpeed*dt*m_camera->rightVector());
        m_camera->setPosition(m_camera->position() + m_rightSpeed*dt*m_camera->rightVector());
        m_camera->roll(3*m_rollSpeed*dt);
    }

    if(!m_forwardIsPressed && !m_backwardIsPressed) {
        if(m_forwardSpeed > 0) {
            m_forwardSpeed -= m_maxSpeed*0.05;
            m_forwardSpeed = std::max(0.0f, m_forwardSpeed);
        } else {
            m_forwardSpeed += m_maxSpeed*0.05;
            m_forwardSpeed = std::min(0.0f, m_forwardSpeed);
        }
    }

    if(!m_rightIsPressed && !m_leftIsPressed) {
        if(m_rightSpeed > 0) {
            m_rightSpeed -= m_maxSpeed*0.05;
            m_rightSpeed = std::max(0.0f, m_rightSpeed);
        } else {
            m_rightSpeed += m_maxSpeed*0.05;
            m_rightSpeed = std::min(0.0f, m_rightSpeed);
        }
    }

    if(!m_rollLeftIsPressed && !m_rollRightIsPressed) {
        if(m_rollSpeed > 0) {
            m_rollSpeed -= m_maxSpeed*0.05;
            m_rollSpeed = std::max(0.0f, m_rollSpeed);
        } else {
            m_rollSpeed += m_maxSpeed*0.05;
            m_rollSpeed = std::min(0.0f, m_rollSpeed);
        }
    }
}

void FlyModeNavigator::setMaxSpeed(float maxSpeed)
{
    if (m_maxSpeed == maxSpeed)
        return;

    m_maxSpeed = maxSpeed;
    emit maxSpeedChanged(maxSpeed);
}

void FlyModeNavigator::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape) {
        setAcceptHoverEvents(false);
        setFocus(false);
        showMouse();
    }
    if(event->key() == Qt::Key_Shift) m_shiftPressed = true;
    if(event->key() == Qt::Key_W) {
        m_forwardIsPressed = true;
        if(m_forwardSpeed < 0) m_forwardSpeed = 0;
        m_forwardSpeed += m_maxSpeed*0.2;
        m_forwardSpeed = std::min(m_forwardSpeed, m_maxSpeed);
    }
    if(event->key() == Qt::Key_S) {
        m_backwardIsPressed = true;
        if(m_forwardSpeed > 0) m_forwardSpeed = 0;
        m_forwardSpeed -= m_maxSpeed*0.2;
        m_forwardSpeed = std::max(m_forwardSpeed, -m_maxSpeed);
    }

    if(event->key() == Qt::Key_D) {
        m_rightIsPressed = true;
        if(m_rightSpeed < 0) m_rightSpeed = 0;
        m_rightSpeed += m_maxSpeed*0.2;
        m_rightSpeed = std::min(m_rightSpeed, m_maxSpeed);
    }
    if(event->key() == Qt::Key_A) {
        m_leftIsPressed = true;
        if(m_rightSpeed > 0) m_rightSpeed = 0;
        m_rightSpeed -= m_maxSpeed*0.2;
        m_rightSpeed = std::max(m_rightSpeed, -m_maxSpeed);
    }

    if(event->key() == Qt::Key_Q) {
        m_rollLeftIsPressed = true;
        if(m_rollSpeed < 0) m_rollSpeed = 0;
        m_rollSpeed += m_maxSpeed*0.2;
        m_rollSpeed = std::min(m_rollSpeed, m_maxSpeed);
    }

    if(event->key() == Qt::Key_E) {
        m_rollRightIsPressed = true;
        if(m_rollSpeed > 0) m_rollSpeed = 0;
        m_rollSpeed -= m_maxSpeed*0.2;
        m_rollSpeed = std::max(m_rollSpeed, -m_maxSpeed);
    }
}

void FlyModeNavigator::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift) m_shiftPressed = false;
    if(event->key() == Qt::Key_W) {
        m_forwardIsPressed = false;
    }
    if(event->key() == Qt::Key_S) {
        m_backwardIsPressed = false;
    }
    if(event->key() == Qt::Key_D) {
        m_rightIsPressed = false;
    }
    if(event->key() == Qt::Key_A) {
        m_leftIsPressed = false;
    }
    if(event->key() == Qt::Key_Q) {
        m_rollLeftIsPressed = false;
    }
    if(event->key() == Qt::Key_E) {
        m_rollRightIsPressed = false;
    }
}

QWindow *FlyModeNavigator::getCurrentWindow() {
    QQuickItem * item = this;
    QWindow *window;
    bool exitNext = false;
    while(true) {
        item = item->parentItem();

        if(exitNext) break;

        window = qobject_cast<QWindow*>(item->parent());
        if(window) {
            return window;
        }

        if(item->parentItem() == 0) {
            exitNext = true;
        }
    }

    return nullptr;
}

void FlyModeNavigator::showMouse() {
    QWindow *window = getCurrentWindow();
    if(window) {
        QPoint middlePos = absolutePosition(QPoint(width()/2,height()/2));
        QCursor::setPos(middlePos);
        QCursor cursor;
        cursor.setShape(Qt::ArrowCursor);
        window->setCursor(cursor);
    }
}

void FlyModeNavigator::hideAndCenterMouse() {
    QWindow *window = getCurrentWindow();
    if(window) {
        QPoint middlePos = absolutePosition(QPoint(width()/2,height()/2));
        QCursor::setPos(middlePos);
        QCursor cursor;
        cursor.setShape(Qt::BlankCursor);
        window->setCursor(cursor);
    }
}

QPoint FlyModeNavigator::absolutePosition(QPoint p) {
    QPointF pointFloat(p.x(), p.y());
    QQuickItem * item = this;
    QWindow *window;
    bool exitNext = false;
    while(true) {
        pointFloat += item->position();
        item = item->parentItem();

        if(exitNext) break;

        window = qobject_cast<QWindow*>(item->parent());
        if(window) {
            return QPoint(pointFloat.x(), pointFloat.y())+window->position();
        }

        if(item->parentItem() == 0) {
            exitNext = true;
        }
    }

    return QPoint(pointFloat.x(), pointFloat.y());
}

void FlyModeNavigator::hoverMoveEvent(QHoverEvent *event)
{
    if(!m_camera) {
        return;
    }

    QPointF deltaF = event->posF() - event->oldPosF();
    if(deltaF == -lastMoveEvent) return;
    lastMoveEvent = deltaF;
    float thetaAboutRight = deltaF.y()/10.;
    float thetaAboutUp = deltaF.x()/10.;

    // First remove all tilt so panning is not biased. Note that y is flipped
    // m_camera->pan(thetaAboutUp, QVector3D(0,-1,0));
    m_camera->pan(thetaAboutUp);
    m_camera->tilt(thetaAboutRight);


    hideAndCenterMouse();
}

void FlyModeNavigator::mousePressEvent(QMouseEvent *event)
{
    setAcceptHoverEvents(true);
    setFocus(true);
    hideAndCenterMouse();
}
