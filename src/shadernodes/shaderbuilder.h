#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "shadernode.h"
#include "shaderoutput.h"

#include <QObject>
#include <QQmlListProperty>
#include <QUrl>
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
    Q_PROPERTY(QUrl sourceFile READ sourceFile WRITE setSourceFile NOTIFY sourceFileChanged)
    Q_PROPERTY(QString finalShader READ finalShader NOTIFY finalShaderChanged)
    Q_PROPERTY(QQmlListProperty<ShaderOutput> outputs READ outputs)

public:
    explicit ShaderBuilder(QObject *parent = 0);

    QString source() const;
    QString finalShader();

    QQmlListProperty<ShaderOutput> outputs();
    QVariantMap uniforms() const;
    void addUniform(ShaderNode *node, const QString &propertyName, const QString &identifier,
                    const QVariant &value, QMetaProperty metaProperty);

    QUrl sourceFile() const;

signals:
    void sourceChanged(QString source);
    void finalShaderChanged();
    void uniformsChanged();

    void sourceFileChanged(QUrl sourceFile);

public slots:
    void setSource(QString source);
    void triggerOutputChange();
    void updateUniform(int i);

    void setSourceFile(QUrl sourceFile);

private:
    QString glslType(QVariant value) const;

    QString m_source;
    QList<ShaderOutput*> m_outputs;
    QList<UniformValue> m_uniforms;
    QList<QSignalMapper*> m_mappers;
    QUrl m_sourceFile;
};

#endif // SHADERBUILDER_H
