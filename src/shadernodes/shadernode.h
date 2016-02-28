#ifndef SHADERNODE_H
#define SHADERNODE_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QQmlListProperty>
#include <QJSValue>
#include <QSignalMapper>

class ShaderBuilder;
class VariantShaderNode;

class ShaderNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString initialization READ initialization WRITE setInitialization NOTIFY initializationChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(QString header READ header WRITE setHeader NOTIFY headerChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)
    Q_PROPERTY(QQmlListProperty<VariantShaderNode> variantNodes READ variantNodes)
    Q_CLASSINFO("DefaultProperty", "variantNodes")

public:
    explicit ShaderNode(QObject *parent = 0);

    Q_INVOKABLE virtual QString generateHeader() const;
    Q_INVOKABLE virtual QString generateBody() const;

    Q_INVOKABLE QString convert(QString targetType, QString identifier = QString()) const;
    Q_INVOKABLE QList<VariantShaderNode *> uniformDependencies() const;
    Q_INVOKABLE QString glslTypeFromVariant(QVariant value) const;
    Q_INVOKABLE QString resolve(ShaderNode *node, QString targetType = QString());

    QString name() const;
    QString type() const;
    QString initialization() const;
    QString result() const;
    QString header() const;
    QQmlListProperty<VariantShaderNode> variantNodes();
    QString identifier() const;
    void reset() const;
    void setup(ShaderBuilder* shaderBuilder);

    ShaderBuilder *shaderBuilder() const;
    void setShaderBuilder(ShaderBuilder *shaderBuilder);

signals:
    void nameChanged(QString name);
    void typeChanged(QString type);
    void initializationChanged(QString initialization);
    void resultChanged(QString result);
    void headerChanged(QString header);
    void identifierChanged(QString identifier);

public slots:
    void setName(QString name);
    void setType(QString type);
    void setInitialization(QString initialization);
    void setResult(QString result);
    void setHeader(QString header);

protected:
    mutable bool m_hasGeneratedHeader = false;
    mutable bool m_hasGeneratedBody = false;
    mutable bool m_hasSetup = false;

private:
    QString m_name;
    QString m_type;
    QString m_initialization;
    QString m_result;
    QString m_resolvedResult;
    QString m_header;
    QList<ShaderNode*> m_dependencies;
    QList<VariantShaderNode*> m_variantNodes;
    QString m_identifier;
    ShaderBuilder *m_shaderBuilder = nullptr;
    QSignalMapper mapper;
};

#endif // SHADERNODE_H
