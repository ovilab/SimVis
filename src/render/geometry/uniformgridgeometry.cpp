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
    int m_vertexCount = 0;
public:
    UniformGridVertexDataFunctor(int vertexCount) { m_vertexCount = vertexCount; }
    void setVertexCount(int vertexCount) { m_vertexCount = vertexCount; }

    QByteArray operator ()() Q_DECL_OVERRIDE
    {
        const quint32 vertexSize = (3+3)*sizeof(float);
        QByteArray verticesData;
        verticesData.resize(vertexSize*m_vertexCount);
        float *verticesPtr = reinterpret_cast<float*>(verticesData.data());
        int verticesPerDimension = cbrt(m_vertexCount); // Since m_vertexCount comes from resolution^3, this will always work
        int N = verticesPerDimension;
        int nHalf = 0.5*N;
        int nHalfSquared = nHalf*nHalf;

        int count = 0;
        for(int i=0; i<N; i++) {
            float x0 = -nHalf + i;
            float x1 = -nHalf + i+1;
            x0 = translate(x0);
            x1 = translate(x1);
            float dx = x1-x0;

            for(int j=0; j<N; j++) {
                float y0 = -nHalf + j;
                float y1 = -nHalf + j+1;
                y0 = translate(y0);
                y1 = translate(y1);
                float dy = y1-y0;

                for(int k=0; k<N; k++) {
                    float z0 = -nHalf + k;
                    float z1 = -nHalf + k+1;
                    z0 = translate(z0);
                    z1 = translate(z1);
                    float dz = z1-z0;

                    if(true || x0*x0 + y0*y0 + z0*z0 < nHalfSquared) {
                        *verticesPtr++ = x0;
                        *verticesPtr++ = y0;
                        *verticesPtr++ = z0;
                        *verticesPtr++ = dx;
                        *verticesPtr++ = dy;
                        *verticesPtr++ = dz;
                        count+=6;
                    }
                }
            }
        }

        verticesData.resize(count*sizeof(float));
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

    const quint32 stride = (3+3) * sizeof(float); // 6 floats, x y z, dx dy dz

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

    m_dataFunctor = new UniformGridVertexDataFunctor(vertexCount());
    QByteArray gridPoints = (*m_dataFunctor)();
    int realVertexCount = gridPoints.size() / (6*sizeof(float));

    m_positionAttribute->setCount(realVertexCount);
    m_deltaAttribute->setCount(realVertexCount);

    m_vertexBuffer->setBufferFunctor(QBufferFunctorPtr(m_dataFunctor));
    addAttribute(m_positionAttribute);
    addAttribute(m_deltaAttribute);
}

UniformGridGeometry::UniformGridGeometry(Qt3DCore::QNode *parent)
    : QGeometry(parent)
{
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

int UniformGridGeometry::resolution() const
{
    return m_resolution;
}

void UniformGridGeometry::setResolution(int resolution)
{
    if (m_resolution == resolution)
        return;

    m_resolution = resolution;
    emit resolutionChanged(resolution);
}