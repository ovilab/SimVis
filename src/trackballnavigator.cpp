#include "trackballnavigator.h"
#include "camera.h"
#include <cmath>

TrackballNavigator::TrackballNavigator(Camera *camera, QQuickItem *parent) :
    Navigator(camera, parent)
{
    qDebug() << "Created trackball";
}

TrackballNavigator::~TrackballNavigator()
{

}

void TrackballNavigator::keyPressEvent(QKeyEvent *event)
{

}

void TrackballNavigator::keyReleaseEvent(QKeyEvent *event)
{

}

void TrackballNavigator::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "Released mouse";
}

void TrackballNavigator::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePosition = QVector2D(event->pos().x(), event->pos().y());
    qDebug() << "Pressed mouse at " << m_lastMousePosition;
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

//    float pan = m_camera->pan();
//    float tilt = m_camera->tilt();

//    pan += deltaPan;
//    tilt += deltaTilt;
//    tilt = std::max(-90.0f, std::min(90.0f, tilt));

//    m_camera->setPan(pan);
//    m_camera->setTilt(tilt);

//    qDebug() << "Moved mouse in navigator. Pan: " << pan << " and tilt: " << tilt;
}

void TrackballNavigator::wheelEvent(QWheelEvent *event)
{

}

void TrackballNavigator::touchEvent(QTouchEvent *event)
{
    qDebug() << "Touched";
}

