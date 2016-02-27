#ifndef SHADEROUTPUT_H
#define SHADEROUTPUT_H

#include "shadernode.h"

#include <QObject>

class ShaderOutput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(ShaderNode* value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit ShaderOutput(QObject *parent = 0);

    QString name() const;
    QString type() const;
    ShaderNode* value() const;

signals:
    void nameChanged(QString name);
    void typeChanged(QString type);
    void valueChanged(ShaderNode* value);

public slots:
    void setName(QString name);
    void setType(QString type);

    void setValue(ShaderNode* value);

private:

    QString m_name;
    QString m_type;
    ShaderNode* m_value = nullptr;
};

#endif // SHADEROUTPUT_H
