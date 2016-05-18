#ifndef SHADERBUILDER_H
#define SHADERBUILDER_H

#include "shadernode.h"
#include "shaderoutput.h"

#include <QOpenGLShader>
#include <QObject>
#include <QQmlListProperty>
#include <QUrl>
#include <QVariantList>

#include <Qt3DCore/QNode>
#include <Qt3DRender/QMaterial>

using namespace Qt3DRender;

class VariantShaderNode;

struct UniformValue
{
    ShaderNode *node;
    QString propertyName;
    QString identifier;
    QVariant value;
    QString type;
    QParameter *parameter;
};

class ShaderBuilder : public Qt3DCore::QNode
{
    Q_OBJECT
    Q_ENUMS(ShaderType)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QUrl sourceFile READ sourceFile WRITE setSourceFile NOTIFY sourceFileChanged)
    Q_PROPERTY(QString finalShader READ finalShader NOTIFY finalShaderChanged)
    Q_PROPERTY(QQmlListProperty<ShaderOutput> inputs READ inputs CONSTANT)
    Q_PROPERTY(QQmlListProperty<ShaderOutput> outputs READ outputs CONSTANT)
    Q_PROPERTY(ShaderType shaderType READ shaderType WRITE setShaderType NOTIFY shaderTypeChanged)
    Q_PROPERTY(QMaterial* material READ material WRITE setMaterial NOTIFY materialChanged)

public:
    explicit ShaderBuilder(QNode *parent = 0);
    virtual ~ShaderBuilder();

    QString source() const;
    QString finalShader();

    QQmlListProperty<ShaderOutput> inputs();
    QQmlListProperty<ShaderOutput> outputs();

    QVariantMap uniforms() const;
    void addUniform(ShaderNode *node, const QString &propertyName, const QString &identifier,
                    const QVariant &value, QMetaProperty metaProperty);

    QUrl sourceFile() const;

    enum class ShaderType {
        Vertex,
        Fragment,
        Geometry,
        Compute,
        TessellationControl,
        TessellationEvaluation
    };

    ShaderType shaderType() const
    {
        return m_shaderType;
    }

    QMaterial* material() const
    {
        return m_material;
    }

signals:
    void sourceChanged(QString source);
    void finalShaderChanged();
    void uniformsChanged();

    void sourceFileChanged(QUrl sourceFile);

    void shaderTypeChanged(ShaderType shaderType);

    void materialChanged(QMaterial* material);

public slots:
    void setSource(QString source);
    void triggerOutputChange();
    void updateUniform(int i);

    void setSourceFile(QUrl sourceFile);

    void setShaderType(ShaderType shaderType)
    {
        if (m_shaderType == shaderType)
            return;

        m_shaderType = shaderType;
        emit shaderTypeChanged(shaderType);
    }

    void setMaterial(QMaterial* material)
    {
        if (m_material == material)
            return;

        m_material = material;
        emit materialChanged(material);
    }

private:
    QString glslType(QVariant value) const;
    void clear();

    QString m_source;
    QList<ShaderOutput*> m_inputs;
    QList<ShaderOutput*> m_outputs;
    QList<UniformValue> m_uniforms;
    QList<QSignalMapper*> m_mappers;
    QUrl m_sourceFile;
    ShaderType m_shaderType = ShaderType::Vertex;
    QMaterial* m_material = nullptr;
    QList<QParameter*> m_parameters;
};

#endif // SHADERBUILDER_H
