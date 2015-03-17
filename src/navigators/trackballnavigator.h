#ifndef TRACKBALLNAVIGATOR_H
#define TRACKBALLNAVIGATOR_H

#include "../core/navigator.h"
class Visualizer;
class TrackballNavigator : public Navigator
{
    Q_OBJECT
    Q_PROPERTY(float zoomSensitivity READ zoomSensitivity WRITE setZoomSensitivity NOTIFY zoomSensitivityChanged)

public:
    explicit TrackballNavigator(Camera *camera = 0, QQuickItem *parent = 0);
~TrackballNavigator();
float zoomSensitivity() const;

public slots:
void setZoomSensitivity(float arg);

signals:
void zoomSensitivityChanged(float arg);

protected:
    QVector2D m_lastMousePosition;
    float m_zoomSensitivity = 1.0;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void touchEvent(QTouchEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // TRACKBALLNAVIGATOR_H
