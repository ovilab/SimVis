#include "trackballnavigator.h"
#include "../core/camera.h"
#include <cmath>

TrackballNavigator::TrackballNavigator(Camera *camera, QQuickItem *parent) :
    Navigator(camera, parent)
{
    qDebug() << "Created trackball";
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
    m_lastMousePosition = QVector2D(event->pos().x(), event->pos().y());
}

void TrackballNavigator::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D currentMousePosition = QVector2D(event->pos().x(), event->pos().y());

    QVector2D delta = currentMousePosition - m_lastMousePosition;
    m_lastMousePosition = currentMousePosition;

    delta.setX( delta.x() / width());
    delta.setY( delta.y() / height());

    float deltaPan = -delta.x() * 100;
    float deltaTilt = -delta.y() * 100;

    m_camera->panAboutViewCenter(deltaPan);
    m_camera->tiltAboutViewCenter(deltaTilt);
}

void TrackballNavigator::wheelEvent(QWheelEvent *event)
{
    float effectiveSensitivity = m_zoomSensitivity / 1080.0; // Typical deltaY is 120, scale by factor 540
    float deltaY = -event->angleDelta().y() * effectiveSensitivity;
    float factor = exp(deltaY);
    m_camera->setPosition(m_camera->position()*factor);
}

void TrackballNavigator::touchEvent(QTouchEvent *event)
{
    qDebug() << "Touched";
}

