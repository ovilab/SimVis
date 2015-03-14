#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QQuickItem>
#include <QQuickFramebufferObject>

class Renderable;
class Simulator;
class VisualizerRenderer : public QQuickFramebufferObject::Renderer
{
protected:
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void render();
    void synchronize(QQuickFramebufferObject *);
private:
    QList<Renderable*> m_renderables;
};

class Visualizer : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Simulator* simulator READ simulator WRITE setSimulator NOTIFY simulatorChanged)
public:
    Visualizer();
    ~Visualizer();

    virtual VisualizerRenderer *createRenderer() const override;
    Simulator* simulator() const;
public slots:
    void setSimulator(Simulator* arg);

    void test();
signals:
    void simulatorChanged(Simulator* arg);
private:
    Simulator* m_simulator;

    friend class VisualizerRenderer;
};

#endif // VISUALIZER_H
