#ifndef VARIATIONGROUP_H
#define VARIATIONGROUP_H

#include "shadernode.h"
#include "variationnode.h"

class VariationGroup : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QVariant target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QString property READ propertyName WRITE setPropertyName NOTIFY propertyChanged)
    Q_PROPERTY(QQmlListProperty<VariationNode> nodes READ nodes CONSTANT)
    Q_CLASSINFO("DefaultProperty", "nodes")
public:
    VariationGroup(QNode *parent = 0);

    QQmlListProperty<VariationNode> nodes()
    {
        return QQmlListProperty<VariationNode>(this, m_nodes);
    }

    QVariant target() const
    {
        return m_target;
    }
    QString propertyName() const
    {
        return m_property;
    }

public slots:
    void setTarget(QVariant target)
    {
        if (m_target == target)
            return;

        m_target = target;
        emit targetChanged(target);
    }
    void setPropertyName(QString property)
    {
        if (m_property == property)
            return;

        m_property = property;
        emit propertyChanged(property);
    }

signals:
    void targetChanged(QVariant target);
    void propertyChanged(QString propertyName);

private:
    QVariant m_target;
    QString m_property;
    QList<VariationNode*> m_nodes;

    friend class ShaderNode;

    QT3D_CLONEABLE(VariationGroup)
};

#endif // VARIATIONGROUP_H
