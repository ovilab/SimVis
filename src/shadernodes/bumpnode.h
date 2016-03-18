#ifndef BUMPNODE_H
#define BUMPNODE_H

#include "shadernode.h"

#include <QQmlComponent>

// TODO add support for non QQmlComponent heights

class BumpNode : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QVariant strength READ strength WRITE setStrength NOTIFY strengthChanged)
    Q_PROPERTY(QVariant height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QVariant vectorPlus READ vectorPlus WRITE setVectorPlus NOTIFY vectorPlusChanged)
    Q_PROPERTY(QVariant vectorMinus READ vectorMinus WRITE setVectorMinus NOTIFY vectorMinusChanged)
public:
    explicit BumpNode(Qt3DCore::QNode *parent = 0);

    QVariant strength() const;
    QVariant vectorPlus() const;
    QVariant vectorMinus() const;
    QVariant height() const;

    virtual bool setup(ShaderBuilder *shaderBuilder, QString tempIdentifier) override;

public slots:
    void setStrength(QVariant strength);
    void setVectorPlus(QVariant vectorPlus);
    void setVectorMinus(QVariant vectorMinus);
    void setHeight(QVariant height);

signals:
    void strengthChanged(QVariant strength);
    void vectorPlusChanged(QVariant vectorPlus);
    void vectorMinusChanged(QVariant vectorMinus);
    void heightChanged(QVariant height);

private:
    QVariant m_strength;

    QVariant m_vectorPlus;
    QVariant m_vectorMinus;

    ShaderNode m_plusNode;
    ShaderNode m_minusNode;
    ShaderNode m_centerNode;
    QVariant m_height;

    QT3D_CLONEABLE(BumpNode)
};

#endif // BUMPNODE_H
