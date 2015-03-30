#ifndef SHADERNODEOUTPUT_H
#define SHADERNODEOUTPUT_H

#include <QObject>

class ShaderNodeOutput : public QObject
{
    Q_OBJECT
public:
    explicit ShaderNodeOutput(QObject *parent = 0);
    ~ShaderNodeOutput();

signals:

public slots:
};

#endif // SHADERNODEOUTPUT_H
