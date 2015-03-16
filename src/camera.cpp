#include "camera.h"
#include <QDateTime>

QMatrix4x4 Camera::projectionMatrix()
{
    // Reset projection
    m_projectionMatrix.setToIdentity();

    // Set perspective projection
    m_projectionMatrix.perspective(m_fieldOfView, m_aspectRatio, m_nearPlane, m_farPlane);

    return m_projectionMatrix;
}

void Camera::setProjectionMatrix(const QMatrix4x4 &projectionMatrix)
{
    m_projectionMatrix = projectionMatrix;
}

QMatrix4x4 Camera::modelViewMatrix()
{
    m_modelViewMatrix.setToIdentity();
    if(!m_fixedPosition) {
        m_modelViewMatrix.translate(m_position);
    }

    m_modelViewMatrix.rotate(90, 1, 0, 0);
    m_modelViewMatrix.rotate(m_tilt, 1, 0, 0);
    m_modelViewMatrix.rotate(m_pan, 0, 0, 1);

    return m_modelViewMatrix;
}

void Camera::setModelViewMatrix(const QMatrix4x4 &modelViewMatrix)
{
    m_modelViewMatrix = modelViewMatrix;
}

bool Camera::fixedPosition() const
{
    return m_fixedPosition;
}

float Camera::fieldOfView() const
{
    return m_fieldOfView;
}

float Camera::farPlane() const
{
    return m_farPlane;
}

float Camera::nearPlane() const
{
    return m_nearPlane;
}

void Camera::setPosition(QVector3D arg)
{
    if (m_position == arg)
        return;

    m_position = arg;
    emit positionChanged(arg);
}

void Camera::setTilt(float arg)
{
    if (m_tilt == arg)
        return;

    m_tilt = arg;
    emit tiltChanged(arg);
}

void Camera::setPan(float arg)
{
    if (m_pan == arg)
        return;

    m_pan = arg;
    emit panChanged(arg);
}

void Camera::setRoll(float arg)
{
    if (m_roll == arg)
        return;

    m_roll = arg;
    emit rollChanged(arg);
}

void Camera::setFixedPosition(bool arg)
{
    if (m_fixedPosition == arg)
        return;

    m_fixedPosition = arg;
    emit fixedPositionChanged(arg);
}

void Camera::setFieldOfView(float arg)
{
    if (m_fieldOfView == arg)
        return;

    m_fieldOfView = arg;
    emit fieldOfViewChanged(arg);
}

void Camera::setFarPlane(float arg)
{
    if (m_farPlane == arg)
        return;

    m_farPlane = arg;
    emit farPlaneChanged(arg);
}

void Camera::setNearPlane(float arg)
{
    if (m_nearPlane == arg)
        return;

    m_nearPlane = arg;
    emit nearPlaneChanged(arg);
}

void Camera::setAspectRatio(float arg)
{
    if (m_aspectRatio == arg)
        return;

    m_aspectRatio = arg;
    emit aspectRatioChanged(arg);
}

Camera::Camera(QObject *parent) :
    QObject(parent)
{

}

Camera::~Camera()
{

}

QVector3D Camera::position() const
{
    return m_position;
}

float Camera::tilt() const
{
    return m_tilt;
}

float Camera::pan() const
{
    return m_pan;
}

float Camera::roll() const
{
    return m_roll;
}

QVector3D Camera::forwardVector()
{
    float x = cos(m_pan*DEGTORAD)*cos(m_tilt*DEGTORAD);
    float y = sin(m_pan*DEGTORAD)*cos(m_tilt*DEGTORAD);
    float z = sin(m_tilt*DEGTORAD);
    return QVector3D(x,y,z);
}

QVector3D Camera::upVector()
{
    QVector3D forwardVector = this->forwardVector();
    float x = -forwardVector.z()*forwardVector.x()/sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
    float y = -forwardVector.z()*forwardVector.y()/sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
    float z = sqrt(forwardVector.x()*forwardVector.x() + forwardVector.y()*forwardVector.y());
    return QVector3D(x,y,z);
}

QVector3D Camera::rightVector()
{
    QVector3D forwardVector = this->forwardVector();
    QVector3D upVector = this->upVector();
    return QVector3D::crossProduct(forwardVector, upVector);
}
