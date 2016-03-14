#include "pointgeometry.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferfunctor.h>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>

using namespace Qt3DRender;

class PointVertexDataFunctor : public Qt3DRender::QBufferFunctor
{
public:
    PointVertexDataFunctor()
    {
    }

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        const int verticesCount = 1;
        // vec3 pos
        const quint32 vertexSize = 3 * sizeof(float);

        QByteArray verticesData;
        verticesData.resize(vertexSize*verticesCount);
        float *verticesPtr = reinterpret_cast<float*>(verticesData.data());
        *verticesPtr++ = 0.0;
        *verticesPtr++ = 0.0;
        *verticesPtr++ = 0.0;
        return verticesData;
    }

    bool operator ==(const QBufferFunctor &other) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(other);
        return true;
    }

    QT3D_FUNCTOR(PointVertexDataFunctor)

private:
};

class PointIndexDataFunctor : public QBufferFunctor
{
public:
    PointIndexDataFunctor()
    {
    }

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        const int indexCount = 1;
        const int indexSize = sizeof(quint16);
        QByteArray indicesBytes;
        indicesBytes.resize(indexCount * indexSize);
        quint16 *indicesPtr = reinterpret_cast<quint16*>(indicesBytes.data());
        *indicesPtr++ = 0;
        return indicesBytes;
    }

    bool operator ==(const QBufferFunctor &other) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(other);
        return true;
    }

    QT3D_FUNCTOR(PointIndexDataFunctor)

private:
};

void PointGeometry::init()
{
    m_positionAttribute = new QAttribute(this);
    m_indexAttribute = new QAttribute(this);
    m_vertexBuffer = new QBuffer(QBuffer::VertexBuffer, this);
    m_indexBuffer = new QBuffer(QBuffer::IndexBuffer, this);

    // vec3 pos, vec2 tex, vec3 normal
    const quint32 elementSize = 3;
    const quint32 stride = elementSize * sizeof(float);
    const int vertexCount = 1;
    const int indexCount = 1;

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setDataType(QAttribute::Float);
    m_positionAttribute->setDataSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(vertexCount);

    m_indexAttribute->setAttributeType(QAttribute::IndexAttribute);
    m_indexAttribute->setDataType(QAttribute::UnsignedShort);
    m_indexAttribute->setBuffer(m_indexBuffer);
    m_indexAttribute->setCount(indexCount);

    m_vertexBuffer->setBufferFunctor(QBufferFunctorPtr(new PointVertexDataFunctor()));
    m_indexBuffer->setBufferFunctor(QBufferFunctorPtr(new PointIndexDataFunctor()));

    addAttribute(m_positionAttribute);
    addAttribute(m_indexAttribute);
}

PointGeometry::PointGeometry(Qt3DCore::QNode *parent)
    : QGeometry(parent)
{
    init();
}

void PointGeometry::updateVertices()
{
}

/*!
 * Updates the indices based on rings and slices.
 */
void PointGeometry::updateIndices()
{
}
