#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "shadernode.h"
#include "shaderoutput.h"

#include <QObject>
#include <QQmlListProperty>
#include <QVariantList>

class VariantShaderNode;

struct UniformValue
{
    ShaderNode *node;
    QString propertyName;
    QString identifier;
    QVariant value;
    QString type;
};

class ShaderBuilder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString finalShader READ finalShader NOTIFY finalShaderChanged)
    Q_PROPERTY(QQmlListProperty<ShaderOutput> outputs READ outputs)

public:
    explicit ShaderBuilder(QObject *parent = 0);

    QString source() const;
    QString finalShader();

    QQmlListProperty<ShaderOutput> outputs();
    QList<VariantShaderNode *> uniformDependencies() const;
    QVariantMap uniforms() const;
    void addUniform(ShaderNode *node, const QString &propertyName, const QString &identifier,
                    const QVariant &value, const QString &type, QMetaProperty metaProperty);

signals:
    void sourceChanged(QString source);
    void finalShaderChanged();
    void uniformsChanged();

public slots:
    void setSource(QString source);
    void receiveOutputChange();
    void updateUniform(int i);

private:
    QString m_source;
    QList<ShaderOutput*> m_outputs;
    QList<UniformValue> m_uniforms;
    QList<QSignalMapper*> m_mappers;
};

#endif // SHADERBUILDER_H
