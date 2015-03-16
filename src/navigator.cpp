#include "navigator.h"
#include "camera.h"

Navigator::Navigator()
{

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
    if (m_camera == arg)
        return;

    m_camera = arg;
    emit cameraChanged(arg);
}

