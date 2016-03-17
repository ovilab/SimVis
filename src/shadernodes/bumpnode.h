#ifndef BUMPNODE_H
#define BUMPNODE_H

#include "shadernode.h"

#include <QQmlComponent>

// TODO add support for non QQmlComponent heights

class BumpNode : public ShaderNode
{
    Q_OBJECT
    Q_PROPERTY(QVariant strength READ strength WRITE setStrength NOTIFY strengthChanged)
    Q_PROPERTY(QQmlComponent* height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QVariant vectorPlus READ vectorPlus WRITE setVectorPlus NOTIFY vectorPlusChanged)
    Q_PROPERTY(QVariant vectorMinus READ vectorMinus WRITE setVectorMinus NOTIFY vectorMinusChanged)
public:
    explicit BumpNode(Qt3DCore::QNode *parent = 0);

    QVariant strength() const;
    QVariant vectorPlus() const;
    QVariant vectorMinus() const;
    QQmlComponent* height() const;

public slots:
    void setStrength(QVariant strength);
    void setVectorPlus(QVariant vectorPlus);
    void setVectorMinus(QVariant vectorMinus);
    void setHeight(QQmlComponent* height);

signals:
    void strengthChanged(QVariant strength);
    void vectorPlusChanged(QVariant vectorPlus);
    void vectorMinusChanged(QVariant vectorMinus);
    void heightChanged(QQmlComponent* height);

private:
    QVariant m_strength;

    QVariant m_vectorPlus;
    QVariant m_vectorMinus;

    ShaderNode *m_plusNode = nullptr;
    ShaderNode *m_minusNode = nullptr;
    ShaderNode *m_centerNode = nullptr;
    QQmlComponent* m_height = nullptr;

    QT3D_CLONEABLE(BumpNode)
};

#endif // BUMPNODE_H
