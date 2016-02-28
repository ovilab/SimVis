#ifndef SHADERGROUP_H
#define SHADERGROUP_H

#include <QObject>
#include <QVariantList>
#include <QQmlListProperty>

#include "shadernode.h"

class ShaderGroup : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ShaderNode> nodes READ nodes CONSTANT)
    Q_CLASSINFO("DefaultProperty", "nodes")
public:
    explicit ShaderGroup(QObject *parent = 0);

    QQmlListProperty<ShaderNode> nodes()
    {
        return QQmlListProperty<ShaderNode>(this, m_nodes);
    }

private:
    QList<ShaderNode*> m_nodes;

    friend class ShaderNode;
};

#endif // SHADERGROUP_H
