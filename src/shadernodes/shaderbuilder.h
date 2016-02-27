#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "shadernode.h"
#include "shaderoutput.h"

#include <QObject>
#include <QQmlListProperty>
#include <QVariantList>

class VariantShaderNode;

class ShaderBuilder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString finalShader READ finalShader NOTIFY finalShaderChanged)
    Q_PROPERTY(QQmlListProperty<ShaderOutput> outputs READ outputs)

public:
    explicit ShaderBuilder(QObject *parent = 0);

    QString source() const;
    QString finalShader() const;

    QQmlListProperty<ShaderOutput> outputs();
    QList<VariantShaderNode *> uniformDependencies() const;

signals:
    void sourceChanged(QString source);
    void finalShaderChanged();

public slots:
    void setSource(QString source);
    void receiveOutputChange();

private:
    QString m_source;
    QList<ShaderOutput*> m_outputs;
};

#endif // SHADERBUILDER_H
