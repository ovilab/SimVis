#include "navigator.h"
#include "camera.h"

Navigator::Navigator(Camera *camera, QQuickItem *parent) :
    QQuickItem(parent)
{
    m_camera = camera;
    setAcceptedMouseButtons(Qt::AllButtons);
    setAcceptHoverEvents(true);
    qDebug() << "Created navigator with parent: " << parent << " and camera " << camera;
}

Navigator::~Navigator()
{

}

Camera *Navigator::camera() const
{
    return m_camera;
}

void Navigator::setCamera(Camera *arg)
{
    qDebug() << "Setting camera: " << arg;
    if (m_camera == arg)
        return;

    m_camera = arg;
    emit cameraChanged(arg);
}

