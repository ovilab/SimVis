#include "shaderbuilder.h"

#include <QRegularExpression>
#include <QDebug>

ShaderBuilder::ShaderBuilder(QObject *parent) : QObject(parent)
{
}

QString ShaderBuilder::source() const
{
    return m_source;
}

QString ShaderBuilder::finalShader() const
{
    qDebug() << "Requesting final shader";
    // Verify all that all outputs have values
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            qWarning() << "ERROR: ShaderDefintion output " << output->name() << " has no value";
            return QString();
        }
    }

    QString contents = "";
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

QList<ShaderNode*> ShaderBuilder::uniformDependencies() const
{
    QList<ShaderNode *> uniforms;
    for(const ShaderOutput *output : m_outputs) {
        ShaderNode *value = output->value();
        if(!value) {
            continue;
        }
        uniforms.append(value->uniformDependencies());
    }
    return uniforms;
}

void ShaderBuilder::setSource(QString source)
{
    if (m_source == source)
        return;

    m_source = source;
    emit finalShaderChanged();
    emit sourceChanged(source);
}
