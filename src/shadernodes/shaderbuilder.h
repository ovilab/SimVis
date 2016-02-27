#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "shadernode.h"
#include "shaderoutput.h"

#include <QObject>
#include <QVariantList>

class ShaderBuilder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString finalShader READ finalShader NOTIFY finalShaderChanged)
    Q_PROPERTY(QVariantList outputs READ outputs WRITE setOutputs NOTIFY outputsChanged)

public:
    explicit ShaderBuilder(QObject *parent = 0);

    QString source() const;
    QString finalShader() const;

    QVariantList outputs() const;
    QList<ShaderNode *> uniformDependencies() const;

signals:
    void sourceChanged(QString source);
    void finalShaderChanged(QString finalShader);
    void outputsChanged(QVariantList outputs);

public slots:
    void setSource(QString source);
    void setOutputs(QVariantList outputs);

private:
    void resolveOutputs();
    void updateFinalShader();

    QString m_source;
    QString m_finalShader;
    QVariantList m_outputs;
    QList<ShaderOutput*> m_resolvedOutputs;
};

#endif // SHADERBUILDER_H
