#ifndef SHADERNODE_H
#define SHADERNODE_H

#include <QQuickItem>
#include <QStringList>

class ShaderNode : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* input READ input WRITE setInput NOTIFY inputChanged)
    Q_PROPERTY(QObject* output READ output WRITE setOutput NOTIFY outputChanged)
    Q_PROPERTY(QStringList inputs READ inputs WRITE setInputs NOTIFY inputsChanged)
    Q_PROPERTY(QStringList outputs READ outputs WRITE setOutputs NOTIFY outputsChanged)
    Q_PROPERTY(QString shaderSource READ shaderSource WRITE setShaderSource NOTIFY shaderSourceChanged)

public:
    explicit ShaderNode(QQuickItem *parent = 0);
    ~ShaderNode();

    QObject* input() const;
    QObject* output() const;
    QString shaderSource() const;

    QStringList inputs() const;
    QStringList outputs() const;

signals:

    void inputChanged(QObject* arg);
    void outputChanged(QObject* arg);
    void shaderSourceChanged(QString arg);
    void inputsChanged(QStringList arg);
    void outputsChanged(QStringList arg);

public slots:
    void setInput(QObject* arg);
    void setOutput(QObject* arg);
    void setShaderSource(QString arg);
    void setInputs(QStringList arg);
    void setOutputs(QStringList arg);

private:
    QObject* m_input;
    QObject* m_output;
    QString m_shaderSource;
    QStringList m_inputs;
    QStringList m_outputs;
};

#endif // SHADERNODE_H
