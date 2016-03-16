#include "shaderbuilder.h"

#include "shaderutils.h"

#include <QRegularExpression>
#include <QDebug>
#include <QMetaProperty>
#include <QQmlEngine>
#include <QQmlFile>
#include <QFile>
#include <QColor>

#include <Qt3DRender/QParameter>
#include <Qt3DRender/QTexture2D>

using Qt3DRender::QTexture2D;

ShaderBuilder::ShaderBuilder(QObject *parent) : QObject(parent)
{
}

QString ShaderBuilder::source() const
{
    return m_source;
}

QString ShaderBuilder::finalShader()
{
    for(QSignalMapper *mapper : m_mappers) {
        disconnect(this, 0, mapper, SLOT(map()));
        disconnect(mapper, SIGNAL(mapped(int)), this, SLOT(updateUniform(int)));
    }
    qDeleteAll(m_mappers);
    if(m_material) {
        for(UniformValue &uniformValue : m_uniforms) {
            m_material->removeParameter(uniformValue.parameter);
            delete uniformValue.parameter;
        }
    }
    m_uniforms.clear();
    // Verify all that all outputs have values
    for(ShaderOutput *output : m_outputs) {
        bool success = output->node()->setup(this);
        if(!success) {
            qWarning() << "ERROR: ShaderBuilder::finalShader(): One of the shader nodes failed during setup.";
            return QString();
        }
    }

    QString header = "";
    header += "\n// ------  begin generated header  ------\n\n";
    for(ShaderOutput *output : m_outputs) {
        header += output->node()->generateHeader();
    }
    header += "\n// ------          uniforms        ------\n\n";
    for(const UniformValue &uniform : m_uniforms) {
        header += "uniform " + uniform.type + " " + uniform.identifier + ";\n";
    }
    header += "\n// ------   end generated header   ------\n\n";

    QString body = "";
    body += "\n// ------   begin generated body   ------\n\n";
    for(ShaderOutput *output : m_outputs) {
        body += output->node()->generateBody();
    }
    body += "\n// ------    output assignments    ------\n\n";
    for(ShaderOutput *output : m_outputs) {
        body += output->name()
                + " = "
                + ShaderUtils::convert(output->node()->type(),
                                       output->type(),
                                       output->node()->identifier())
                + ";\n";
    }
    body += "\n// ------    end generated body    ------\n\n";
    body.replace(QRegularExpression("\n"), "\n    ");

    QString contents = m_source;
    contents.replace(QRegularExpression("#pragma shadernodes header"), header);
    contents.replace(QRegularExpression("#pragma shadernodes body"), body);

    for(ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->node();
        if(!value) {
            continue;
        }
        value->reset();
    }

    if(m_material) {
        for(UniformValue &uniformValue : m_uniforms) {
            m_material->addParameter(uniformValue.parameter);
        }
    }

    return contents;
}

QQmlListProperty<ShaderOutput> ShaderBuilder::outputs()
{
    return QQmlListProperty<ShaderOutput>(this, m_outputs);
}

QVariantMap ShaderBuilder::uniforms() const
{
    QVariantMap result;
    for(const UniformValue &uniform : m_uniforms) {
        result[uniform.identifier] = uniform.value;
    }
    return result;
}

void ShaderBuilder::addUniform(ShaderNode *node, const QString &propertyName, const QString &identifier,
                               const QVariant &value, QMetaProperty metaProperty)
{
    UniformValue uniform;
    uniform.node = node;
    uniform.propertyName = propertyName;
    uniform.identifier = identifier;
    uniform.value = value;
    uniform.type = glslType(value);
    QParameter* param = new QParameter();
    param->setName(identifier);
    if(value.type() == QVariant::String) {
        param->setValue(QColor(value.toString()));
    } else {
        param->setValue(value);
    }
    uniform.parameter = param;
    m_uniforms.append(uniform);

    QSignalMapper *mapper = new QSignalMapper;
    mapper->setMapping(node, m_uniforms.length() - 1);
    const QByteArray signal = '2' + metaProperty.notifySignal().methodSignature();
    connect(node, signal, mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(updateUniform(int)));
    m_mappers.append(mapper);
}

QUrl ShaderBuilder::sourceFile() const
{
    return m_sourceFile;
}

QQmlListProperty<ShaderOutput> ShaderBuilder::inputs()
{
    return QQmlListProperty<ShaderOutput>(this, m_inputs);
}

void ShaderBuilder::updateUniform(int i)
{
    UniformValue &uniform = m_uniforms[i];
    QByteArray propertyNameArray = uniform.propertyName.toUtf8();
    QVariant value = uniform.node->property(propertyNameArray.constData());;
    uniform.value = value;
    uniform.parameter->setValue(value);
    QString type = glslType(value);
    if(type != uniform.type) {
        uniform.type = type;
        emit finalShaderChanged();
    }
    emit uniformsChanged();
}

void ShaderBuilder::setSourceFile(QUrl sourceFile)
{
    if (m_sourceFile == sourceFile)
        return;

    m_sourceFile = sourceFile;

    QString fileName = QQmlFile::urlToLocalFileOrQrc(sourceFile);
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    setSource(file.readAll());

    emit sourceFileChanged(sourceFile);
}

void ShaderBuilder::setSource(QString source)
{
    if (m_source == source)
        return;
    m_source = source;
    emit finalShaderChanged();
    emit sourceChanged(source);
}

void ShaderBuilder::triggerOutputChange()
{
    emit finalShaderChanged();
}

QString ShaderBuilder::glslType(QVariant value) const
{
    return ShaderUtils::glslType(value);
}
