#include "uniformgridgeometry.h"
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qbufferfunctor.h>
#include <Qt3DRender/qattribute.h>
#include <qmath.h>
#include <QVector3D>

using namespace Qt3DRender;

float translate(float x) {
    float y = 0;

    for(int i = 0; i<fabs(x); i++) {
        if(i<6) {
            y += i;
        } else if(i<8) {
            y += 2*i;
        } else if(i<10) {
            y += 4*i;
        } else if(i<12) {
            y += 8*i;
        } else if(i<16) {
            y += 16*i;
        } else  {
            y += 32*i;
        }
    }
    if(x>0) return y;
    else return -y;
}

class UniformGridVertexDataFunctor : public Qt3DRender::QBufferFunctor
{
    int m_nR, m_nPhi, m_nTheta;
public:
    UniformGridVertexDataFunctor(int inx, int ny, int nz) { m_nR = nz; m_nPhi = ny; m_nTheta = nz; }

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        const quint32 vertexSize = (3+3)*sizeof(float);
        QByteArray verticesData;
        int vertexCount = m_nR*m_nPhi*m_nTheta;
        verticesData.resize(vertexSize*vertexCount);
        float *verticesPtr = reinterpret_cast<float*>(verticesData.data());

        for(int i=0; i<m_nPhi; i++) {
            for(int j=0; j<m_nTheta; j++) {
                for(int k=0; k<m_nR; k++) {
                    *verticesPtr++ = i;
                    *verticesPtr++ = j;
                    *verticesPtr++ = k;
                    *verticesPtr++ = m_nR;
                    *verticesPtr++ = m_nPhi;
                    *verticesPtr++ = m_nTheta;
                }
            }
        }

//        for(int i=0; i<vertexCount; i++) {
//            *verticesPtr++ = i;
//            *verticesPtr++ = i;
//            *verticesPtr++ = i;
//            *verticesPtr++ = m_nR;
//            *verticesPtr++ = m_nPhi;
//            *verticesPtr++ = m_nTheta;
//        }

        return verticesData;
    }

    bool operator ==(const QBufferFunctor &other) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(other);
        return true;
    }

    QT3D_FUNCTOR(UniformGridVertexDataFunctor)
};

void UniformGridGeometry::init()
{
    m_positionAttribute = new QAttribute(this);
    m_deltaAttribute = new QAttribute(this);

    m_vertexBuffer = new QBuffer(QBuffer::VertexBuffer, this);

    const quint32 stride = (3+3)*sizeof(float);

    m_positionAttribute->setName(QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setDataType(QAttribute::Float);
    m_positionAttribute->setDataSize(3);
    m_positionAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(stride);

    m_deltaAttribute->setName("delta");
    m_deltaAttribute->setDataType(QAttribute::Float);
    m_deltaAttribute->setDataSize(3);
    m_deltaAttribute->setByteOffset(3*sizeof(float));
    m_deltaAttribute->setAttributeType(QAttribute::VertexAttribute);
    m_deltaAttribute->setBuffer(m_vertexBuffer);
    m_deltaAttribute->setByteStride(stride);

    m_dataFunctor = new UniformGridVertexDataFunctor(m_nR, m_nPhi, m_nTheta);

    m_positionAttribute->setCount(vertexCount());
    m_deltaAttribute->setCount(vertexCount());

    m_vertexBuffer->setBufferFunctor(QBufferFunctorPtr(m_dataFunctor));
    addAttribute(m_positionAttribute);
    addAttribute(m_deltaAttribute);
}

UniformGridGeometry::UniformGridGeometry(Qt3DCore::QNode *parent)
    : QGeometry(parent)
{
    // TODO if m_nR * m_nPhi * m_nTheta >= 32768 the program sometimes crashes on Ubuntu
    m_nR = 24;
    m_nPhi = 32;
    m_nTheta = 24;
    init();
}

void UniformGridGeometry::updateVertices()
{
    qWarning() << "UniformGridGeometry::updateVertices() not implemented";
}

/*!
 * Updates the indices based on rings and slices.
 */
void UniformGridGeometry::updateIndices()
{
    qWarning() << "UniformGridGeometry::updateIndices()";
}
