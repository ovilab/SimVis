#include "shadergraph.h"
#include "shadernodeinput.h"
#include "shadernodeoutput.h"
#include "shaderbinding.h"

#include <QRegularExpression>
#include <QUuid>

ShaderGraph::ShaderGraph(QQuickItem *parent) : QQuickItem(parent)
{
    setObjectName("a" + QUuid::createUuid().toString().replace(QRegularExpression("[{}-]"), "").left(8) + "_");
}

ShaderGraph::~ShaderGraph()
{

}

QOpenGLShader::ShaderType ShaderGraph::shaderType()
{

}

QString ShaderGraph::shaderSource() const
{
    return m_shaderSource;
}

QQmlListProperty<ShaderNode> ShaderGraph::shaderNodes() const
{
    return m_shaderNodes;
}

void ShaderGraph::setShaderType(QOpenGLShader::ShaderType arg)
{
    if (m_shaderType == arg)
        return;

    m_shaderType = arg;
    emit shaderTypeChanged(arg);
}

void ShaderGraph::setShaderNodes(QQmlListProperty<ShaderNode> arg)
{
    if (m_shaderNodes == arg)
        return;

    m_shaderNodes = arg;
    emit shaderNodesChanged(arg);
}

void ShaderGraph::componentComplete()
{
    qDebug() << "Component complete!";

    m_shaderSource = "";

    const QMetaObject* meta = metaObject();
    for(int i = meta->propertyOffset(); i < meta->propertyCount(); i++) {
        QMetaProperty property = meta->property(i);
        QString propertyName = property.name();
        m_shaderSource += "uniform " + objectName() + propertyName + " = " + property.read(this).toString() + ";\n";
    }

    m_shaderSource += "\n";
    m_shaderSource += "int main() {\n";
    for(ShaderNode* shaderNode : findChildren<ShaderNode*>()) {
        qDebug() << "Got node!";
        QString nodeSource = shaderNode->shaderSource();
        const QMetaObject* inputMeta = shaderNode->input()->metaObject();
        for(int i = inputMeta->propertyOffset(); i < inputMeta->propertyCount(); i++) {
            QMetaProperty property = inputMeta->property(i);
            QString propertyName = property.name();
            nodeSource = nodeSource.replace(QRegularExpression("\\b" + propertyName + "\\b"),
                                            shaderNode->objectName() + propertyName);

            if(property.type() == QVariant::Double) {
                m_shaderSource += "    float ";
            }
            m_shaderSource += shaderNode->objectName() + QString(property.name());
            bool foundBinding = false;
            for(ShaderBinding* binding : shaderNode->findChildren<ShaderBinding*>()) {
                ShaderNode* target = shaderNode;
                QString targetProperty = binding->targetProperty();
                if(targetProperty != propertyName) {
                    continue;
                }
                QObject* source = binding->source();
                if(!source) {
                    ShaderNode* parent = qobject_cast<ShaderNode*>(binding->parent());
                    if(parent) {
                        source = parent;
                    } else {
                        qWarning() << "Source not set in binding and parent is not a node. "
                                   << "Binding will be ignored.";
                        continue;
                    }
                }
                if(source == target) {
                    qWarning() << "Source and target cannot be same object in binding.";
                    continue;
                }

                QString sourceName = source->objectName();
                QString sourceProperty = binding->sourceProperty();

                m_shaderSource += " = " + sourceName + sourceProperty + ";\n";
                foundBinding = true;
            }
            if(!foundBinding) {
                m_shaderSource += ";\n";
            }
        }

        const QMetaObject* outputMeta = shaderNode->output()->metaObject();
        for(int i = outputMeta->propertyOffset(); i < outputMeta->propertyCount(); i++) {
            QMetaProperty property = outputMeta->property(i);
            QString propertyName = property.name();
            nodeSource = nodeSource.replace(QRegularExpression("\\b" + propertyName + "\\b"),
                                            shaderNode->objectName() + propertyName);

            if(property.type() == QVariant::Double) {
                m_shaderSource += "    float ";
            }
            m_shaderSource += shaderNode->objectName() + QString(property.name()) + ";\n";
        }

        m_shaderSource += "    " + nodeSource + "\n";
    }
    m_shaderSource += "}\n";
//    qDebug() << m_shaderSource;
    emit shaderSourceChanged(m_shaderSource);
}

