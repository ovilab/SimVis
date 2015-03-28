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

QVector2D TrackballNavigator::scaledTouchPosition(QVector2D touchPosition) {
    return QVector2D(touchPosition.x()/width(), touchPosition.y()/height());
}

void TrackballNavigator::moved(QVector2D delta)
{
    float deltaPan = -delta.x() * 200;
    float deltaTilt = -delta.y() * 200;

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
    m_touch1Position = scaledTouchPosition(QVector2D(event->pos().x(), event->pos().y()));
}

void TrackballNavigator::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D touch1Position = scaledTouchPosition(QVector2D(event->pos().x(), event->pos().y()));
    QVector2D delta = m_touch1Position - touch1Position;
    m_touch1Position = touch1Position;
    moved(-delta);
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
    if(numberOfTouches >= 1) {
        const QTouchEvent::TouchPoint *touch1Point = &event->touchPoints().first();
        const QTouchEvent::TouchPoint *touch2Point = 0;
        if(numberOfTouches >= 2) {
            touch2Point = &event->touchPoints().at(1);
        }

        if(event->touchPointStates() & Qt::TouchPointMoved) {
            // QVector2D touch1Position = scaledTouchPosition(QVector2D(touch1Point->pos().x(), touch1Point->pos().y()));
            QVector2D delta1 = scaledTouchPosition(QVector2D(touch1Point->pos() - touch1Point->lastPos()));

            if(numberOfTouches >= 2) {
                // Only do pinching if we didn't release a touch
                if(!(event->touchPointStates() & Qt::TouchPointReleased)) {
                    // QVector2D touch2Position = scaledTouchPosition(QVector2D(touch2Point->pos().x(), touch2Point->pos().y()));
                    float initialDistance = scaledTouchPosition(QVector2D(touch1Point->lastPos() - touch2Point->lastPos())).length();
                    float currentDistance = scaledTouchPosition(QVector2D(touch1Point->pos() - touch2Point->pos())).length();
                    float deltaDistance = currentDistance - initialDistance;
                    float factor = exp(-2.0*deltaDistance);
                    m_camera->setPosition(m_camera->position()*factor);
                }

                QVector2D delta2 = scaledTouchPosition(QVector2D(touch2Point->pos() - touch2Point->lastPos()));
                if(touch1Point->state() == Qt::TouchPointMoved) moved(delta1);
                if(touch2Point->state() == Qt::TouchPointMoved) moved(delta2);
            } else {
                moved(delta1);
            }
        }
    }
}

