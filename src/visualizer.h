#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QQuickItem>
#include <QQuickFramebufferObject>

class Renderable; class Simulator; class SimulatorWorker; class Camera; class Navigator;

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
    Q_PROPERTY(Navigator* navigator READ navigator WRITE setNavigator NOTIFY navigatorChanged)
public:
    Visualizer();
    ~Visualizer();

    Simulator* simulator() const;
    Camera* camera();

    QColor backgroundColor() const;

    Navigator* navigator();

public slots:
    void setSimulator(Simulator* arg);
    void setCamera(Camera* arg);
    void setBackgroundColor(QColor arg);
    void setNavigator(Navigator* arg);

private slots:
    void synchronizeWorker(SimulatorWorker* worker);

signals:
    void simulatorChanged(Simulator* arg);
    void cameraChanged(Camera* arg);
    void backgroundColorChanged(QColor arg);
    void navigatorChanged(Navigator* arg);

private:
    Simulator* m_simulator = 0;
    Camera* m_camera = 0;
    Navigator* m_navigator = 0;
    QColor m_backgroundColor = QColor(0,0,0,0);

    friend class VisualizerRenderer;

protected:
    virtual VisualizerRenderer *createRenderer() const override;
};

#endif // VISUALIZER_H
