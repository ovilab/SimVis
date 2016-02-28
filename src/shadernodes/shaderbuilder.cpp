#include "shaderbuilder.h"

#include <QRegularExpression>
#include <QDebug>
#include <QMetaProperty>

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
    m_uniforms.clear();
    // Verify all that all outputs have values
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            qWarning() << "ERROR: ShaderDefintion output " << output->name() << " has no value";
            return QString();
        }
        output->value()->setup(this);
        output->value()->setShaderBuilder(const_cast<ShaderBuilder*>(this));
    }

    QString contents = "";
    contents += "\n// ------ begin generated uniforms ------\n\n";
    for(const UniformValue &uniform : m_uniforms) {
        contents += "uniform " + uniform.type + " " + uniform.identifier + ";\n";
    }
    contents += "\n// ------  end generated uniforms  ------\n\n";
    contents += "\n// ------  begin generated header  ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        contents += value->generateHeader();
    }
    contents += "\n// ------   end generated header   ------\n\n";
    contents += "\n// ------  begin generated outputs ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        contents += "out " + output->type() + " " + output->name() + ";\n";
    }
    contents += "\n// ------  end generated outputs   ------\n\n";

    QString setup = "";
    setup += "\n// ------   begin generated body   ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode* value = output->value();
        setup += value->generateBody();
    }
    setup += "\n// ------    end generated body    ------\n\n";
    setup += "\n// ------ begin output assignments ------\n\n";
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode* value = output->value();
        setup += output->name() + " = " + value->convert(output->type()) + ";\n";
    }
    setup += "\n// ------  end output assignments  ------\n\n";

    QString matchString = "\\$setupShaderNodes\\(\\);?";

    QRegularExpression indentRegex("^( *)" + matchString, QRegularExpression::PatternOption::MultilineOption);
    QRegularExpressionMatch indentMatch = indentRegex.match(m_source);
    if(indentMatch.hasMatch()) {
        setup.replace(QRegularExpression("\n"), "\n" + indentMatch.captured(1));
    }
    QString originalSource = m_source;
    contents += originalSource.replace(QRegularExpression(matchString), setup);

    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
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
    uniform.type = glslTypeFromVariant(value);
    m_uniforms.append(uniform);

    QSignalMapper *mapper = new QSignalMapper;
    mapper->setMapping(node, m_uniforms.length() - 1);
    const QByteArray signal = '2' + metaProperty.notifySignal().methodSignature();
    connect(node, signal, mapper, SLOT(map()));
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(updateUniform(int)));
    m_mappers.append(mapper);
}

void ShaderBuilder::updateUniform(int i)
{
    UniformValue &uniform = m_uniforms[i];
    QVariant value = uniform.node->property(uniform.propertyName.toStdString().c_str());;
    uniform.value = value;
    QString type = glslTypeFromVariant(value);
    if(type != uniform.type) {
        uniform.type = type;
        emit finalShaderChanged();
    }
    emit uniformsChanged();
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

QString ShaderBuilder::glslTypeFromVariant(QVariant value) const
{
    switch(value.type()) {
    case QVariant::Bool:
        return QString("bool");
        break;
    case QVariant::Int:
        return QString("int");
        break;
    case QVariant::Double:
        return QString("float");
        break;
    case QVariant::Vector2D:
        return QString("vec2");
        break;
    case QVariant::Vector3D:
        return QString("vec3");
        break;
    case QVariant::Vector4D:
        return QString("vec4");
        break;
    default:
        qWarning() << "WARNING: GlslVariantBridge could not identify type" << value.typeName();
        return QString("float");
        break;
    }
}
