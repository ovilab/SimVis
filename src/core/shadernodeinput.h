#ifndef SHADERNODEINPUT_H
#define SHADERNODEINPUT_H

#include <QObject>

class ShaderNodeInput : public QObject
{
    Q_OBJECT
public:
    explicit ShaderNodeInput(QObject *parent = 0);
    ~ShaderNodeInput();

signals:

public slots:
};

#endif // SHADERNODEINPUT_H
