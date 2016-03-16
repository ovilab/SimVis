#ifndef SHADERBUILDERBINDING_H
#define SHADERBUILDERBINDING_H

#include "shadernode.h"

#include <QObject>

class ShaderBuilderBinding : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)

public:
    explicit ShaderBuilderBinding(QObject *parent = 0);

    QString property() const;

signals:
    void propertyChanged(QString property);

public slots:
    void setProperty(QString property);

private:
    QString m_property;

    // ShaderNode interface
public:
    virtual bool setup(ShaderBuilder *shaderBuilder) override;
};

#endif // SHADERBUILDERBINDING_H
