#include "bumpnode.h"
#include "shaderutils.h"

#include <QVector3D>

BumpNode::BumpNode(Qt3DCore::QNode *parent)
    : ShaderNode(parent)
    , m_centerNode(this)
    , m_plusNode(this)
    , m_minusNode(this)
{
    addMapping("center", QVariant::fromValue(&m_centerNode));
    addMapping("plus", QVariant::fromValue(&m_plusNode));
    addMapping("minus", QVariant::fromValue(&m_minusNode));

    m_centerNode.setName("center");
    m_plusNode.setName("plus");
    m_minusNode.setName("minus");

    setStrength(0.0);

    setName("bump");
}

bool BumpNode::setup(ShaderBuilder *shaderBuilder, QString aliasIdentifier)
{
    if(m_hasSetup) {
        return true;
    }

    QString currentIdentifier;
    if(!aliasIdentifier.isEmpty()) {
        currentIdentifier = aliasIdentifier;
    } else {
        currentIdentifier = identifier();
    }

    setType(ShaderUtils::glslType(m_height));

    m_centerNode.clearDependencies();
    ShaderNode *node = qvariant_cast<ShaderNode*>(m_height);
    if(!node) {
        setResult("$height");
    } else {
        setResult("1.0 / 3.0 * ($center + $plus + $minus)");

        node->addMapping("offset", QVector3D(0, 0, 0));
        node->setup(shaderBuilder, m_centerNode.identifier());

        m_centerNode.setType(node->type());
        for(ShaderNode *dependency : node->resolvedDependencies()) {
            m_centerNode.addDependency(dependency);
        }
        m_centerNode.setSourceWithoutNotification(node->resolvedSource());

        double offset = 0.005;

        node->addMapping("offset", QVector3D(offset, offset, offset));
        node->setup(shaderBuilder, m_plusNode.identifier());

        m_plusNode.setType(node->type());
        for(ShaderNode *dependency : node->resolvedDependencies()) {
            m_plusNode.addDependency(dependency);
        }
        m_plusNode.setSourceWithoutNotification(node->resolvedSource());

        node->addMapping("offset", QVector3D(-offset, -offset, -offset));
        node->setup(shaderBuilder, m_minusNode.identifier());

        m_minusNode.setType(node->type());
        for(ShaderNode *dependency : node->resolvedDependencies()) {
            m_minusNode.addDependency(dependency);
        }
        m_minusNode.setSourceWithoutNotification(node->resolvedSource());
    }

    bool success = ShaderNode::setup(shaderBuilder, aliasIdentifier);
    if(!success) {
        return false;
    }
    return true;
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

QVariant BumpNode::height() const
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

void BumpNode::setHeight(QVariant height)
{
    if(m_height == height) {
        return;
    }
    m_height = height;
    emit heightChanged(height);
}
