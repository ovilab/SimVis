#ifndef SHADERNODE_H
#define SHADERNODE_H

#include <QString>
#include <QVariant>
#include <QList>

class ShaderNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString initialization READ initialization WRITE setInitialization NOTIFY initializationChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(QString header READ header WRITE setHeader NOTIFY headerChanged)
    Q_PROPERTY(bool isUniform READ isUniform WRITE setIsUniform NOTIFY isUniformChanged)
    Q_PROPERTY(QList<QVariant> depends READ depends WRITE setDepends NOTIFY dependsChanged)
    Q_PROPERTY(QString identifier READ identifier NOTIFY identifierChanged)

public:
    explicit ShaderNode(QObject *parent = 0);

    QString name() const;
    QString type() const;
    QString initialization() const;
    QString result() const;
    QString header() const;
    bool isUniform() const;
    QList<QVariant> depends() const;
    QString identifier() const;

    Q_INVOKABLE QString generateHeader() const;
    Q_INVOKABLE QString convert(QString targetType) const;
    Q_INVOKABLE QList<QVariant> findUniforms() const;
    Q_INVOKABLE QString generateBody() const;

signals:
    void nameChanged(QString name);
    void typeChanged(QString type);
    void initializationChanged(QString initialization);
    void resultChanged(QString result);
    void headerChanged(QString header);
    void isUniformChanged(bool isUniform);
    void dependsChanged(QList<QVariant> depends);
    void identifierChanged(QString identifier);

public slots:
    void setName(QString name);
    void setType(QString type);
    void setInitialization(QString initialization);
    void setResult(QString result);
    void setHeader(QString header);
    void setIsUniform(bool isUniform);
    void setDepends(QList<QVariant> depends);

private:
    void resolveDependencies();

    QString m_name;
    QString m_type;
    QString m_initialization;
    QString m_result;
    QString m_header;
    bool m_isUniform;
    QList<QVariant> m_depends;
    QList<ShaderNode*> m_resolvedDependencies;
    QString m_identifier;

    mutable bool m_hasGeneratedHeader = false;
    mutable bool m_hasGeneratedBody = false;
};

#endif // SHADERNODE_H
