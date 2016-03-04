#include "shaderbuilder.h"

#include "shaderutils.h"

#include <QRegularExpression>
#include <QDebug>
#include <QMetaProperty>
#include <QQmlEngine>
#include <QQmlFile>
#include <QFile>

ShaderBuilder::ShaderBuilder(QObject *parent) : QObject(parent)
{
}

QString ShaderBuilder::source() const
{
    return m_source;
}

QString ShaderBuilder::finalShader()
{
    qDebug() << "Request for final " << m_outputs.count();
    for(QSignalMapper *mapper : m_mappers) {
        disconnect(this, 0, mapper, SLOT(map()));
        disconnect(mapper, SIGNAL(mapped(int)), this, SLOT(updateUniform(int)));
    }
    qDeleteAll(m_mappers);
    m_uniforms.clear();
    // Verify all that all outputs have values
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        bool success = output->value()->setup(this);
        if(!success) {
            qWarning() << "ShaderBuilder::finalShader(): Shader construction failed.";
            return QString();
        }
    }

    QString contents = "";
    contents += "\n// ------  begin generated header  ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        contents += value->generateHeader();
    }
    contents += "\n// ------          uniforms        ------\n\n";
    for(const UniformValue &uniform : m_uniforms) {
        contents += "uniform " + uniform.type + " " + uniform.identifier + ";\n";
    }
    contents += "\n// ------           inputs         ------\n\n";
    for(const ShaderOutput *input : m_inputs) {
        contents += "cp_in " + input->type() + " " + input->name();

        // TODO add support for other input types and thus other numbers of elements in inputs
        if(m_shaderType == ShaderType::Geometry) {
            contents += "[1]";
        }
        contents += ";\n";
    }
    contents += "\n// ------           outputs        ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        if(output->name() == "cp_FragColor") {
            continue;
        }
        contents += "cp_out " + output->type() + " " + output->name() + ";\n";
    }

    QString setup = "";
    setup += "\n// ------   begin generated body   ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        setup += value->generateBody();
    }
    setup += "\n// ------    output assignments    ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        setup += output->name() + " = " + ShaderUtils::convert(value->type(), output->type(), value->identifier()) + ";\n";
    }
    setup += "\n// ------    end generated body    ------\n\n";

    QString matchString = "\\$setupShaderNodes\\(\\);?";

    QRegularExpression indentRegex("^( *)" + matchString, QRegularExpression::PatternOption::MultilineOption);
    QRegularExpressionMatch indentMatch = indentRegex.match(m_source);
    if(indentMatch.hasMatch()) {
        setup.replace(QRegularExpression("\n"), "\n" + indentMatch.captured(1));
    }
    QString originalSource = m_source;
    originalSource.replace(QRegularExpression(matchString), setup);
    contents += originalSource;

    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        value->reset();
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
    qDebug() << "Source changed!";
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
