#ifndef CAMERA_H
#define CAMERA_H
#define DEGTORAD M_PI/180.0

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QSize>
#include <QTimer>
#include <QQuaternion>
#include <cmath>

class Camera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(float tilt READ tilt WRITE setTilt NOTIFY tiltChanged)
    Q_PROPERTY(float pan READ pan WRITE setPan NOTIFY panChanged)
    Q_PROPERTY(float roll READ roll WRITE setRoll NOTIFY rollChanged)
    Q_PROPERTY(QSize viewportSize READ viewportSize WRITE setViewportSize NOTIFY viewportSizeChanged)
    Q_PROPERTY(QVector3D forwardVector READ forwardVector)
    Q_PROPERTY(QVector3D rightVector READ rightVector)
    Q_PROPERTY(QVector3D upVector READ upVector)
    Q_PROPERTY(bool fixedPosition READ fixedPosition WRITE setFixedPosition NOTIFY fixedPositionChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(float fieldOfView READ fieldOfView WRITE setFieldOfView NOTIFY fieldOfViewChanged)


private:
    QVector3D m_position;
    float m_tilt;
    float m_pan;
    float m_roll;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    bool m_fixedPosition;
    QSize m_viewportSize;
    float m_fieldOfView;
    float m_farPlane;
    float m_nearPlane;
    QVector3D m_forwardVector;
    QVector3D m_upVector;
public:
    explicit Camera(QObject *parent = 0);
    ~Camera();
    QVector3D position() const;
    float tilt() const;
    float pan() const;
    float roll() const;
    QVector3D forwardVector();
    QVector3D upVector();
    QVector3D rightVector();
    QMatrix4x4 projectionMatrix();
    void setProjectionMatrix(const QMatrix4x4 &projectionMatrix);
    QMatrix4x4 modelViewMatrix();
    void setModelViewMatrix(const QMatrix4x4 &modelViewMatrix);
    bool fixedPosition() const;
    QSize viewportSize() const;
    float fieldOfView() const;
    float farPlane() const;
    float nearPlane() const;


public slots:
    void setPosition(QVector3D arg);
    void setTilt(float arg);
    void setPan(float arg);
    void setRoll(float arg);
    void setFixedPosition(bool arg);
    void setViewportSize(QSize arg);
    void setFieldOfView(float arg);
    void setFarPlane(float arg);
    void setNearPlane(float arg);
signals:
    void positionChanged(QVector3D arg);
    void tiltChanged(float arg);
    void panChanged(float arg);
    void rollChanged(float arg);
    void fixedPositionChanged(bool arg);
    void viewportSizeChanged(QSize arg);
    void fieldOfViewChanged(float arg);
    void farPlaneChanged(float arg);
    void nearPlaneChanged(float arg);
};

#endif // CAMERA_H
