#ifndef SHADERGRAPH_H
#define SHADERGRAPH_H

#include "shadernode.h"

#include <QObject>
#include <QOpenGLShader>
#include <QQmlListProperty>
#include <QQuickItem>

class ShaderGraph : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QOpenGLShader::ShaderType shaderType READ shaderType WRITE setShaderType NOTIFY shaderTypeChanged)
    Q_PROPERTY(QString shaderSource READ shaderSource NOTIFY shaderSourceChanged)
//    Q_PROPERTY(QQmlListProperty<ShaderNode> shaderNodes READ shaderNodes WRITE setShaderNodes NOTIFY shaderNodesChanged)
//    Q_CLASSINFO("DefaultProperty", "shaderNodes")

public:
    explicit ShaderGraph(QQuickItem *parent = 0);
    ~ShaderGraph();

    QOpenGLShader::ShaderType shaderType();
    QString shaderSource() const;

    QQmlListProperty<ShaderNode> shaderNodes() const;

signals:
    void shaderTypeChanged(QOpenGLShader::ShaderType arg);
    void shaderSourceChanged(QString arg);

    void shaderNodesChanged(QQmlListProperty<ShaderNode> arg);

public slots:
    void setShaderType(QOpenGLShader::ShaderType arg);
    void setShaderNodes(QQmlListProperty<ShaderNode> arg);

private:
    QOpenGLShader::ShaderType m_shaderType;
    QString m_shaderSource;
    QQmlListProperty<ShaderNode> m_shaderNodes;

    // QQmlParserStatus interface
public:
    void componentComplete();
};

#endif // SHADERGRAPH_H
