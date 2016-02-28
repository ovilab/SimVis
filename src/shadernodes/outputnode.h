#ifndef OUTPUTNODE_H
#define OUTPUTNODE_H

#include "shadernode.h"

#include <QObject>

class OutputNode : public ShaderNode
{
    Q_OBJECT
public:
    explicit OutputNode(QObject *parent = 0);

signals:

public slots:

private:

};

#endif // OUTPUTNODE_H
