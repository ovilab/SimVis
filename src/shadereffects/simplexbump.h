#ifndef SIMPLEXBUMP_H
#define SIMPLEXBUMP_H

#include "../core/shadereffect.h"

class SimplexBump : public ShaderEffect
{
    Q_OBJECT
    Q_PROPERTY(float intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    SimplexBump();
    ~SimplexBump();

    // ShaderEffect interface
    QString fragmentShaderDefines();
    QString vertexShaderDefines();
    QString fragmentShaderLibrary();
    QString vertexShaderLibrary();
    SimplexBump *clone();
    void setUniformValues(QOpenGLShaderProgram &shaderProgram);

    float intensity() const;
    float scale() const;

public slots:
    void setIntensity(float arg);
    void setScale(float arg);

signals:
    void intensityChanged(float arg);
    void scaleChanged(float arg);

private:
    float m_intensity = 1.0;
    float m_scale = 1.0;
};

#endif // SIMPLEXBUMP_H
