#ifndef SHADERBINDING_H
#define SHADERBINDING_H

#include <QObject>

class ShaderBinding : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString targetProperty READ targetProperty WRITE setTargetProperty NOTIFY targetPropertyChanged)
    Q_PROPERTY(QString sourceProperty READ sourceProperty WRITE setSourceProperty NOTIFY sourcePropertyChanged)

public:
    explicit ShaderBinding(QObject *parent = 0);
    ~ShaderBinding();

    QObject* source() const
    {
        return m_source;
    }

    QString targetProperty() const
    {
        return m_targetProperty;
    }

    QString sourceProperty() const
    {
        return m_sourceProperty;
    }

signals:
    void sourceChanged(QObject* arg);
    void targetPropertyChanged(QString arg);
    void sourcePropertyChanged(QString arg);

public slots:
    void setSource(QObject* arg)
    {
        if (m_source == arg)
            return;

        m_source = arg;
        emit sourceChanged(arg);
    }
    void setTargetProperty(QString arg)
    {
        if (m_targetProperty == arg)
            return;

        m_targetProperty = arg;
        emit targetPropertyChanged(arg);
    }
    void setSourceProperty(QString arg)
    {
        if (m_sourceProperty == arg)
            return;

        m_sourceProperty = arg;
        emit sourcePropertyChanged(arg);
    }
private:
    QObject* m_source = 0;
    QString m_targetProperty;
    QString m_sourceProperty;
};

#endif // SHADERBINDING_H
