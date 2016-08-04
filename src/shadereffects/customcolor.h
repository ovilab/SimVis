#ifndef CUSTOMCOLOR_H
#define CUSTOMCOLOR_H

#include <core/shadereffect.h>

class CustomColor : public ShaderEffect
{
    Q_OBJECT
    Q_PROPERTY(QString code READ code WRITE setCode NOTIFY codeChanged)
    QString m_code;

public:
    CustomColor();

    // ShaderEffect interface
public:
    virtual QString fragmentShaderDefines() override;
    virtual QString vertexShaderDefines() override;
    virtual QString fragmentShaderLibrary() override;
    virtual QString vertexShaderLibrary() override;
    virtual CustomColor *clone() override;
    virtual void copyState(ShaderEffect *source) override;
    virtual void beforeRendering(QOpenGLShaderProgram &shaderProgram) override;
    virtual void afterRendering(QOpenGLShaderProgram &shaderProgram) override;
    QString code() const;
public slots:
    void setCode(QString code);
signals:
    void codeChanged(QString code);
};

#endif // CUSTOMCOLOR_H
