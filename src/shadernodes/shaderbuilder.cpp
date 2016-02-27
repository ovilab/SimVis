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
    return m_finalShader;
}

QVariantList ShaderBuilder::outputs() const
{
    return m_outputs;
}

QList<ShaderNode*> ShaderBuilder::uniformDependencies() const
{
    QList<ShaderNode *> uniforms;
    for(const ShaderOutput *output : m_resolvedOutputs) {
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
    updateFinalShader();
    emit sourceChanged(source);
}

void ShaderBuilder::updateFinalShader()
{
    // Verify all that all outputs have values
    for(const ShaderOutput *output : m_resolvedOutputs) {
        ShaderNode *value = output->value();
        if(!value) {
            qWarning() << "ERROR: ShaderDefintion output " << output->name() << " has no value";
            return;
        }
    }

    QString contents = "";
    contents += "\n// ------  begin generated header  ------\n\n";
    for(const ShaderOutput *output : m_resolvedOutputs) {
        ShaderNode *value = output->value();
        contents += value->generateHeader();
    }
    contents += "\n// ------   end generated header   ------\n\n";
    contents += "\n// ------  begin generated outputs ------\n\n";
    for(const ShaderOutput *output : m_resolvedOutputs) {
        contents += "out " + output->type() + " " + output->name() + ";\n";
    }
    contents += "\n// ------  end generated outputs   ------\n\n";

    QString setup = "";
    setup += "\n// ------   begin generated body   ------\n\n";
    for(const ShaderOutput *output : m_resolvedOutputs) {
        ShaderNode* value = output->value();
        setup += value->generateBody();
    }
    setup += "\n// ------    end generated body    ------\n\n";
    setup += "\n// ------ begin output assignments ------\n\n";
    for(const ShaderOutput *output : m_resolvedOutputs) {
        ShaderNode* value = output->value();
        setup += output->name() + " = " + value->convert(output->type()) + ";\n";
    }
    setup += "\n// ------  end output assignments  ------\n\n";

    QString matchString = "\\$setupShaderNodes\\(\\);?";

//    QRegularExpression indentRegex("^( *)" + matchString, QRegularExpression::PatternOption::MultilineOption);
//    QString indent = indentRegex.match(m_source);
//    if(indent && indent.length > 1) {
//        setup = setup.replace(/\n/g, "\n" + indent[1])
//    }
    QString originalSource = m_source;
    contents += originalSource.replace(QRegularExpression(matchString), setup);

    m_finalShader = contents;

    emit finalShaderChanged(m_finalShader);
}

void ShaderBuilder::resolveOutputs()
{
    m_resolvedOutputs.clear();
    for(const QVariant &outputVariant : m_outputs) {
        ShaderOutput* output = qvariant_cast<ShaderOutput*>(outputVariant);
        if(!output) {
            qWarning() << "WARNING: ShaderBuilder: Cannot use" <<  outputVariant << "as output";
            continue;
        }
        m_resolvedOutputs.append(output);
    }
}

void ShaderBuilder::setOutputs(QVariantList outputs)
{
    if (m_outputs == outputs)
        return;

    m_outputs = outputs;

    resolveOutputs();
    updateFinalShader();
    emit outputsChanged(outputs);
}
