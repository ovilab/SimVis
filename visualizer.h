#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QQuickItem>
#include <QQuickFramebufferObject>

class Renderable; class Simulator; class SimulatorWorker; class Camera;

class VisualizerRenderer : public QQuickFramebufferObject::Renderer
{
public:
    Camera *camera() const;
    void setCamera(Camera *camera);

protected:
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void render();
    void synchronize(QQuickFramebufferObject *);
private:
    QList<Renderable*> m_renderables;
    Camera *m_camera = 0;
    QColor m_backgroundColor = QColor(0,0,0,0);
};

class Visualizer : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Simulator* simulator READ simulator WRITE setSimulator NOTIFY simulatorChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
public:
    Visualizer();
    ~Visualizer();

    Simulator* simulator() const;
    Camera* camera();

    QColor backgroundColor() const;

public slots:
    void setSimulator(Simulator* arg);
    void setCamera(Camera* arg);
    void setBackgroundColor(QColor arg);

private slots:
    void synchronizeWorker(SimulatorWorker* worker);
signals:
    void simulatorChanged(Simulator* arg);
    void cameraChanged(Camera* arg);
    void backgroundColorChanged(QColor arg);

private:
    Simulator* m_simulator = 0;
    Camera* m_camera = 0;
    QColor m_backgroundColor = QColor(0,0,0,0);
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual VisualizerRenderer *createRenderer() const override;
    virtual void mousePressEvent(QMouseEvent *event) override;

    friend class VisualizerRenderer;
protected:
    virtual void hoverEnterEvent(QHoverEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
    virtual void hoverLeaveEvent(QHoverEvent *event) override;
};

#endif // VISUALIZER_H
