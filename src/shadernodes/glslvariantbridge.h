#ifndef GLSLVARIANTBRIDGE_H
#define GLSLVARIANTBRIDGE_H

#include <QObject>
#include <QVariant>

class GlslVariantBridge : public QObject
{
    Q_OBJECT
public:
    explicit GlslVariantBridge(QObject *parent = 0);

    Q_INVOKABLE QString type(QVariant value) const;
private:
};

#endif // GLSLVARIANTBRIDGE_H
