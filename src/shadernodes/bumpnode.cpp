#include "bumpnode.h"

#include <QVector3D>

BumpNode::BumpNode(Qt3DCore::QNode *parent)
    : ShaderNode(parent)
{
    addMapping("center", QVector3D(0, 0, 0));
    addMapping("plus", QVector3D(0, 0, 0));
    addMapping("minus", QVector3D(0, 0, 0));

    setName("bump");
    setType("vec3");
    setResult("1.0 / 3.0 * ($(center, vec3) + $(plus, vec3) + $(minus, vec3))");
}

QVariant BumpNode::strength() const
{
    return m_strength;
}

QVariant BumpNode::vectorPlus() const
{
    return m_vectorPlus;
}

QVariant BumpNode::vectorMinus() const
{
    return m_vectorMinus;
}

QQmlComponent *BumpNode::height() const
{
    return m_height;
}

void BumpNode::setStrength(QVariant strength)
{
    if (m_strength == strength)
        return;

    m_strength = strength;
    emit strengthChanged(strength);
}

void BumpNode::setVectorPlus(QVariant vectorPlus)
{
    if (m_vectorPlus == vectorPlus)
        return;

    m_vectorPlus = vectorPlus;
    emit vectorPlusChanged(vectorPlus);
}

void BumpNode::setVectorMinus(QVariant vectorMinus)
{
    if (m_vectorMinus == vectorMinus)
        return;

    m_vectorMinus = vectorMinus;
    emit vectorMinusChanged(vectorMinus);
}

void BumpNode::setHeight(QQmlComponent *delegate)
{
    if (m_height == delegate)
        return;

    if(m_height) {
        removeMapping("center");
        removeMapping("plus");
        removeMapping("minus");
        delete m_centerNode;
        delete m_plusNode;
        delete m_minusNode;
    }

    m_height = delegate;

    QObject *object = m_height->create();
    ShaderNode *node = qobject_cast<ShaderNode*>(object);
    if(node) {
        node->setParent(this);
        m_centerNode = node;
        addMapping("center", QVariant::fromValue(m_centerNode));
    }
    object = m_height->create();
    node = qobject_cast<ShaderNode*>(object);

    double offset = 0.001;

    if(node) {
        node->setParent(this);
        m_plusNode = node;
        m_plusNode->setProperty("offset", QVector3D(offset, offset, offset));
        addMapping("plus", QVariant::fromValue(m_plusNode));
    }
    object = m_height->create();
    node = qobject_cast<ShaderNode*>(object);
    if(node) {
        node->setParent(this);
        m_minusNode = node;
        m_plusNode->setProperty("offset", QVector3D(-offset, -offset, -offset));
        addMapping("minus", QVariant::fromValue(m_minusNode));
    }

    emit heightChanged(delegate);
}
