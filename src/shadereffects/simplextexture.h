#ifndef SIMPLEXTEXTURE_H
#define SIMPLEXTEXTURE_H
#include "../core/shadereffect.h"

class SimplexTexture : public ShaderEffect
{
    Q_OBJECT
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    // ShaderEffect interface
    QString fragmentShaderDefines() override;
    QString vertexShaderDefines() override;
    QString fragmentShaderLibrary() override;
    QString vertexShaderLibrary() override;
    SimplexTexture *clone() override;
    void copyState(ShaderEffect *source) override;
    void setUniformValues(QOpenGLShaderProgram &shaderProgram) override;

    float scale() const;
public slots:
    void setScale(float arg);
signals:
    void scaleChanged(float arg);
private:
    float m_scale = 1.0;
};

#endif // SIMPLEXTEXTURE_H
