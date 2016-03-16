#ifndef SHADERGROUP_H
#define SHADERGROUP_H

#include <QObject>
#include <QVariantList>
#include <QQmlListProperty>

#include <Qt3DCore/QNode>

#include "shadernode.h"

class ShaderGroup : public Qt3DCore::QNode
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<ShaderNode> nodes READ nodes CONSTANT)
    Q_CLASSINFO("DefaultProperty", "nodes")
public:
    explicit ShaderGroup(Qt3DCore::QNode *parent = 0);

    QQmlListProperty<ShaderNode> nodes()
    {
        return QQmlListProperty<ShaderNode>(this, m_nodes);
    }

private:
    QList<ShaderNode*> m_nodes;

    friend class ShaderNode;

    QT3D_CLONEABLE(ShaderGroup)
};

#endif // SHADERGROUP_H
