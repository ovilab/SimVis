#include "trackballnavigator.h"
#include "../core/camera.h"
#include <cmath>

TrackballNavigator::TrackballNavigator(Camera *camera, QQuickItem *parent) :
    Navigator(camera, parent)
{

}

TrackballNavigator::~TrackballNavigator()
{

}

float TrackballNavigator::zoomSensitivity() const
{
    return m_zoomSensitivity;
}

void TrackballNavigator::setZoomSensitivity(float arg)
{
    if (m_zoomSensitivity == arg)
        return;

    m_zoomSensitivity = arg;
    emit zoomSensitivityChanged(arg);
}

void TrackballNavigator::pressed(QVector2D position)
{
    m_lastPosition = position;
}

void TrackballNavigator::moved(QVector2D position)
{
    QVector2D delta = position - m_lastPosition;
    m_lastPosition = position;

    delta.setX( delta.x() / width());
    delta.setY( delta.y() / height());

    float deltaPan = -delta.x() * 100;
    float deltaTilt = -delta.y() * 100;

    m_camera->panAboutViewCenter(deltaPan);
    m_camera->tiltAboutViewCenter(deltaTilt);
}

void TrackballNavigator::keyPressEvent(QKeyEvent *event)
{

}

void TrackballNavigator::keyReleaseEvent(QKeyEvent *event)
{

}

void TrackballNavigator::mouseReleaseEvent(QMouseEvent *event)
{

}

void TrackballNavigator::mousePressEvent(QMouseEvent *event)
{
    QVector2D position = QVector2D(event->pos().x(), event->pos().y());
    pressed(position);
}

void TrackballNavigator::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D position = QVector2D(event->pos().x(), event->pos().y());
    moved(position);
}

void TrackballNavigator::wheelEvent(QWheelEvent *event)
{
    float effectiveSensitivity = m_zoomSensitivity / 1080.0; // Typical deltaY is 120, scale by factor 1080
    float deltaY = -event->angleDelta().y() * effectiveSensitivity;
    float factor = exp(deltaY);
    m_camera->setPosition(m_camera->position()*factor);
}

void TrackballNavigator::touchEvent(QTouchEvent *event)
{
    int numberOfTouches = event->touchPoints().size();
    if(numberOfTouches == 1) {
        const QTouchEvent::TouchPoint &touchPoint = event->touchPoints().first();

        if(event->touchPointStates() == Qt::TouchPointPressed) {
            m_initialTouchId = touchPoint.id();
            QVector2D position = QVector2D(touchPoint.pos().x(), touchPoint.pos().y());
            pressed(position);
        } else if(event->touchPointStates() == Qt::TouchPointMoved && touchPoint.id() == m_initialTouchId) {
            QVector2D position = QVector2D(touchPoint.pos().x(), touchPoint.pos().y());
            moved(position);
        }
    }

}

