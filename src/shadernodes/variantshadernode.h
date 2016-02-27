#ifndef VARIANTSHADERNODE_H
#define VARIANTSHADERNODE_H

#include "shadernode.h"

#include <QObject>

class VariantShaderNode : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    explicit VariantShaderNode(QObject *parent = 0);

    QVariant value() const;
    Q_INVOKABLE virtual QString generateHeader() const override;
    Q_INVOKABLE virtual QString generateBody() const override;

signals:
    void valueChanged(QVariant value);

public slots:
    void setValue(QVariant value);
private:
    QVariant m_value;
};

#endif // VARIANTSHADERNODE_H
