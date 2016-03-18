#ifndef VARIATIONNODE_H
#define VARIATIONNODE_H

#include "shadernode.h"

class VariationNode : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
public:
    VariationNode();
    QVariant value() const
    {
        return m_value;
    }
public slots:
    void setValue(QVariant value)
    {
        if (m_value == value)
            return;

        m_value = value;
        emit valueChanged(value);
    }
signals:
    void valueChanged(QVariant value);
private:
    QVariant m_value;
    QT3D_CLONEABLE(VariationNode)
};

#endif // VARIATIONNODE_H
