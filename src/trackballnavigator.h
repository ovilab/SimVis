#ifndef TRACKBALLNAVIGATOR_H
#define TRACKBALLNAVIGATOR_H
#include "navigator.h"
class Visualizer;
class TrackballNavigator : public Navigator
{
    Q_OBJECT

public:
    explicit TrackballNavigator(Camera *camera = 0, QQuickItem *parent = 0);
    ~TrackballNavigator();
protected:
    QVector2D m_lastMousePosition;
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void touchEvent(QTouchEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // TRACKBALLNAVIGATOR_H
