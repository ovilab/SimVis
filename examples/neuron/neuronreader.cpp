#include "neuronreader.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QQmlFile>
#include "core/camera.h"


NeuronWorker::NeuronWorker()
{
    reset();
}

void NeuronWorker::synchronizeSimulator(Simulator *simulator)
{
    NeuronReader *sim = static_cast<NeuronReader *>(simulator);
    if(sim->m_segmentsAreDirty) {
        m_segments = sim->m_segments;
        m_cylinders = sim->m_cylinders;
        m_spheres = sim->m_spheres;
        sim->m_segmentsAreDirty = false;
    }
}

void NeuronWorker::synchronizeRenderer(Renderable *renderable)
{
    Cylinders *cylinders = qobject_cast<Cylinders*>(renderable);
    if(cylinders) {
        cylinders->setCylinders(m_cylinders);
        cylinders->setDirty(true);
    }

    Spheres *spheres = qobject_cast<Spheres*>(renderable);
    if(spheres) {
        spheres->setPositions(m_spheres);
        spheres->setDirty(true);
    }
}

void NeuronWorker::work()
{
}

void NeuronWorker::reset()
{
    m_segments.clear();
}


NeuronReader::NeuronReader(QQuickItem *parent)
    : Simulator(parent)
{

}

NeuronReader::~NeuronReader()
{

}

QUrl NeuronReader::source() const
{
    return m_source;
}

void NeuronReader::readFile()
{
    QString fileNameString = QQmlFile::urlToLocalFileOrQrc(m_source);
    QFile file(fileNameString);
    file.open(QFile::ReadOnly);
    if(!file.isOpen()) {
        qWarning() << "Could not read file" << fileNameString;
        return;
    }
    QXmlStreamReader reader(&file);
    //    QList<Student> students;
    reader.readNext();
    //Reading from the file
    Segment segment;
    while (!reader.isEndDocument())
    {
        if(reader.isStartElement()) {
//            qDebug() << reader.name() << reader.isStartElement() << reader.isEndElement();
            if(reader.name() == "segment") {
                m_segments.append(segment);
                segment = Segment();
                segment.id = reader.attributes().value("id").toInt();
                if(reader.attributes().hasAttribute("parent")) {
                    segment.parentID = reader.attributes().value("parent").toInt();
                    segment.hasParentID = true;
                }
            }
            if(reader.name() == "parent") {
                segment.parentID = reader.attributes().value("segment").toInt();
                segment.hasParentID = true;
            }
            if(reader.name() == "proximal") {
//                qDebug() << reader.attributes().value("x") << reader.attributes().value("y") << reader.attributes().value("z");
                segment.proximal.setX(reader.attributes().value("x").toDouble());
                segment.proximal.setY(reader.attributes().value("y").toDouble());
                segment.proximal.setZ(reader.attributes().value("z").toDouble());
                segment.proximalWidth = reader.attributes().value("diameter").toDouble();
                segment.hasProximal = true;
            }
            if(reader.name() == "distal") {
//                qDebug() << reader.attributes().value("x") << reader.attributes().value("y") << reader.attributes().value("z") << reader.attributes().value("diameter");
                segment.distal.setX(reader.attributes().value("x").toDouble());
                segment.distal.setY(reader.attributes().value("y").toDouble());
                segment.distal.setZ(reader.attributes().value("z").toDouble());
                segment.distalWidth = reader.attributes().value("diameter").toDouble();
                segment.hasDistal = true;
            }
        }
        reader.readNext();
    }

    m_segments.append(segment);
    for(Segment &segment : m_segments) {
        Segment &s = segment;
        if(!segment.hasProximal && segment.hasParentID) {
            for(Segment &otherSegment : m_segments) {
                if(otherSegment.id == segment.parentID) {
                    Segment &parent = otherSegment;
                    segment.proximal = parent.distal;
                    break;
                }
            }
        }
        if(segment.proximalWidth == 0.0) {
            segment.proximalWidth = segment.distalWidth;
        }
//        qDebug() << s.id << s.parentID << s.proximal << s.distal << s.proximalWidth << s.distalWidth;
    }

    m_cylinders.clear();
    for(const Segment &segment : m_segments) {
        CylinderVBOData cylinder;

        double scaler = 1.0;

        cylinder.vertex1 = segment.proximal * scaler;
        cylinder.vertex2 = segment.distal * scaler;
        cylinder.radius1 = segment.proximalWidth * scaler * 0.5;
        cylinder.radius2 = segment.distalWidth * scaler * 0.5;

        m_cylinders.push_back(cylinder);
    }

//    m_cylinders.clear();
//    CylinderVBOData cylinder;
//    cylinder.vertex1 = QVector3D(1.0, -2.0,  1.0);
//    cylinder.vertex2 = QVector3D(-1.0,  2.0,  -1.0);
//    cylinder.radius1 = 2.4;
//    cylinder.radius2 = 1.2;

//    m_cylinders.push_back(cylinder);


    qDebug() << "Done reading";
    m_segmentsAreDirty = true;
}

void NeuronReader::setSource(QUrl arg)
{
    if (m_source == arg)
        return;

    m_source = arg;
    readFile();
    emit sourceChanged(arg);
}

SimulatorWorker *NeuronReader::createWorker()
{
    return new NeuronWorker();
}

//RenderableRenderer *NeuronRenderable::createRenderer()
//{
//    return new NeuronRenderer();
//}

//const QColor& NeuronRenderable::color() const
//{
//    return m_color;
//}

//void NeuronRenderable::setColor(QColor arg)
//{
//    if (m_color == arg)
//        return;

//    m_color = arg;
//    emit colorChanged(arg);
//}

//QVector3D NeuronRenderable::vectorFromColor(const QColor &color)
//{
//    return QVector3D(color.redF(), color.greenF(), color.blueF());
//}

//NeuronRenderer::NeuronRenderer()
//{
//    m_vboCount = 2;
//}

//void NeuronRenderer::beforeLinkProgram()
//{
//    setShaderFromSourceFile(QOpenGLShader::Vertex, ":/org.compphys.SimVis/renderables/cylinders/cylinders.vsh");
//    setShaderFromSourceFile(QOpenGLShader::Fragment, ":/org.compphys.SimVis/renderables/cylinders/cylinders.fsh");
//}

//void NeuronRenderer::uploadVBOs(NeuronRenderable* neuron)
//{
//    if(neuron->m_segments.size() < 1) {
//        return;
//    }

//    double scale = neuron->scale();
////    double scale = 0.3;
//    QVector<Segment>& segments = neuron->m_segments;
//    QVector<SphereVBOData>& vertices = neuron->m_vertices;
//    QVector<GLuint>& indices = neuron->m_indices;
//    QVector3D color = neuron->vectorFromColor(neuron->color());

//    QVector3D right = m_rightVector;
//    QVector3D up = m_upVector;

//    QVector3D ul = (0.5*up - 0.5*right)*scale;
//    QVector3D ur = (0.5*up + 0.5*right)*scale;
//    QVector3D dl = (-0.5*up - 0.5*right)*scale;
//    QVector3D dr = (-0.5*up + 0.5*right)*scale;

////    QVector<Segment> segments;
////    Segment test;
////    test.proximal = QVector3D(0, 0, 0);
////    test.distal = QVector3D(0.1, 0, 0);
////    segments.push_back(test);

//    int verticesPerPoint = 4*2;
//    int indicesPerPoint = 6*3;

//    int numberOfVertices = segments.size()*verticesPerPoint;
//    vertices.resize(numberOfVertices);
//    indices.resize(segments.size()*indicesPerPoint);

//    for(auto i=0; i<segments.size(); i++) {
//        double scala = 0.1;
//        Segment& segment = segments[i];
//        QVector3D distal = segment.distal * scala;
//        QVector3D proximal = segment.proximal * scala;
//        double aScale = scale * segment.distalWidth;
//        QVector3D tangent = (distal - proximal).normalized();
//        QVector3D projected = tangent - QVector3D::dotProduct(tangent, m_viewVector) * m_viewVector;
//        QVector3D orthogonal = QVector3D::crossProduct(m_viewVector.normalized(), projected.normalized());

//        if(orthogonal.x() < 0) { // ensure normal always points in the same direction
//            orthogonal *= -1.0;
//        }

//        QVector3D l = -0.5 * orthogonal * aScale;
//        QVector3D r = 0.5 * orthogonal * aScale;

//        QVector3D d = -0.5 * tangent * aScale;
//        QVector3D u = 0.5 * tangent * aScale;

//        int n = verticesPerPoint;
//        int m = indicesPerPoint;

//        vertices[n*i + 0].sphereId = i;
//        vertices[n*i + 0].position = distal;
//        vertices[n*i + 0].position += r;
//        vertices[n*i + 0].textureCoord= QVector2D(1,0);

//        vertices[n*i + 1].sphereId = i;
//        vertices[n*i + 1].position = distal;
//        vertices[n*i + 1].position += l;
//        vertices[n*i + 1].textureCoord= QVector2D(-1,0);

//        vertices[n*i + 2].sphereId = i;
//        vertices[n*i + 2].position = proximal;
//        vertices[n*i + 2].position += l;
//        vertices[n*i + 2].textureCoord= QVector2D(-1,0);

//        vertices[n*i + 3].sphereId = i;
//        vertices[n*i + 3].position = proximal;
//        vertices[n*i + 3].position += r;
//        vertices[n*i + 3].textureCoord= QVector2D(1,0);

//        vertices[n*i + 4].sphereId = i;
//        vertices[n*i + 4].position = distal;
//        vertices[n*i + 4].position += u + l;
//        vertices[n*i + 4].textureCoord= QVector2D(-1,1);

//        vertices[n*i + 5].sphereId = i;
//        vertices[n*i + 5].position = distal;
//        vertices[n*i + 5].position += u + r;
//        vertices[n*i + 5].textureCoord= QVector2D(1,1);

//        vertices[n*i + 6].sphereId = i;
//        vertices[n*i + 6].position = proximal;
//        vertices[n*i + 6].position += d + r;
//        vertices[n*i + 6].textureCoord= QVector2D(1,-1);

//        vertices[n*i + 7].sphereId = i;
//        vertices[n*i + 7].position = proximal;
//        vertices[n*i + 7].position += d + l;
//        vertices[n*i + 7].textureCoord= QVector2D(-1,-1);

//        vertices[n*i + 0].color = color;
//        vertices[n*i + 1].color = color;
//        vertices[n*i + 2].color = color;
//        vertices[n*i + 3].color = color;

//        vertices[n*i + 4].color = color;
//        vertices[n*i + 5].color = color;
//        vertices[n*i + 6].color = color;
//        vertices[n*i + 7].color = color;

//        indices [m*i + 0] = n*i+0;
//        indices [m*i + 1] = n*i+1;
//        indices [m*i + 2] = n*i+2;

//        indices [m*i + 3] = n*i+2;
//        indices [m*i + 4] = n*i+3;
//        indices [m*i + 5] = n*i+0;

//        indices [m*i + 6] = n*i+0;
//        indices [m*i + 7] = n*i+1;
//        indices [m*i + 8] = n*i+4;

//        indices [m*i + 9] = n*i+4;
//        indices [m*i + 10] = n*i+5;
//        indices [m*i + 11] = n*i+0;

//        indices [m*i + 12] = n*i+2;
//        indices [m*i + 13] = n*i+3;
//        indices [m*i + 14] = n*i+6;

//        indices [m*i + 15] = n*i+6;
//        indices [m*i + 16] = n*i+7;
//        indices [m*i + 17] = n*i+2;
//    }

//    // Transfer vertex data to VBO 0
//    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
//    glFunctions()->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SphereVBOData), &vertices.front(), GL_STATIC_DRAW);

//    // Transfer index data to VBO 1
//    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);
//    glFunctions()->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.front(), GL_STATIC_DRAW);
//}

//void NeuronRenderer::synchronize(Renderable *renderable)
//{
//    NeuronRenderable* neuronRenderable = static_cast<NeuronRenderable*>(renderable);

//    m_upVector = neuronRenderable->camera()->upVector().normalized();
//    m_viewVector = neuronRenderable->camera()->viewVector().normalized();
//    m_rightVector = QVector3D::crossProduct(m_viewVector, m_upVector);

//    if(!m_isInitialized) {
//        generateVBOs();
//        m_isInitialized = true;
//    }

//    uploadVBOs(neuronRenderable);

//    m_vertexCount = neuronRenderable->m_vertices.count();
//    m_indexCount = neuronRenderable->m_indices.count();
//}

//void NeuronRenderer::render()
//{
//    qDebug() << "Counting vertices";

//    if(m_vertexCount == 0) {
//        return;
//    }

//    qDebug() << "Rendering!";

//    // Tell OpenGL which VBOs to use
//    glFunctions()->glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
//    glFunctions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vboIds[1]);

//    // Offset for position
//    quintptr offset = 0;

//    // Tell OpenGL programmable pipeline how to locate vertex position data
//    int sphereIdLocation = program().attributeLocation("a_sphereId");
//    program().enableAttributeArray(sphereIdLocation);
//    glFunctions()->glVertexAttribPointer(sphereIdLocation, 1, GL_FLOAT, GL_FALSE, sizeof(SphereVBOData), (const void *)offset);

//    offset += sizeof(GLfloat);

//    // Tell OpenGL programmable pipeline how to locate vertex position data
//    int vertexLocation = program().attributeLocation("a_position");
//    program().enableAttributeArray(vertexLocation);
//    glFunctions()->glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVBOData), (const void *)offset);

//    // Offset for texture coordinate
//    offset += sizeof(QVector3D);

//    // Tell OpenGL programmable pipeline how to locate vertex color data
//    int colorLocation = program().attributeLocation("a_color");
//    program().enableAttributeArray(colorLocation);
//    glFunctions()->glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, sizeof(SphereVBOData), (const void *)offset);

//    // Offset for texture coordinate
//    offset += sizeof(QVector3D);

//    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
//    int texcoordLocation = program().attributeLocation("a_texcoord");
//    program().enableAttributeArray(texcoordLocation);
//    glFunctions()->glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(SphereVBOData), (const void *)offset);
//    // Draw cube geometry using indices from VBO 1

//    glFunctions()->glEnable(GL_DEPTH_TEST);
//    glFunctions()->glDisable(GL_CULL_FACE);
//    glFunctions()->glDepthMask(GL_TRUE);
//    glFunctions()->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
//    glFunctions()->glDisable(GL_DEPTH_TEST);

//    program().disableAttributeArray(vertexLocation);
//    program().disableAttributeArray(colorLocation);
//    program().disableAttributeArray(texcoordLocation);
//}
