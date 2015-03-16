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
};

class Visualizer : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Simulator* simulator READ simulator WRITE setSimulator NOTIFY simulatorChanged)
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
public:
    Visualizer();
    ~Visualizer();

    Simulator* simulator() const;
    Camera* camera();

public slots:
    void setSimulator(Simulator* arg);
    void setCamera(Camera* arg);

private slots:
    void synchronizeWorker(SimulatorWorker* worker);
signals:
    void simulatorChanged(Simulator* arg);
    void cameraChanged(Camera* arg);

private:
    Simulator* m_simulator = 0;
    Camera* m_camera = 0;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual VisualizerRenderer *createRenderer() const override;

    friend class VisualizerRenderer;
    virtual void mousePressEvent(QMouseEvent *event) override;

    // QQuickItem interface
protected:
    virtual void hoverEnterEvent(QHoverEvent *event) override;
    virtual void hoverMoveEvent(QHoverEvent *event) override;
    virtual void hoverLeaveEvent(QHoverEvent *event) override;

    // QQuickFramebufferObject interface
};

#endif // VISUALIZER_H
