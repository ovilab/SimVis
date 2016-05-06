#include "spheredata.h"

#include <QVector3D>

using Qt3DRender::QBuffer;

SphereData::SphereData(QNode *parent)
    : QNode(parent)
    , m_buffer(new QBuffer(QBuffer::VertexBuffer, this))
{
}

Qt3DRender::QBuffer *SphereData::buffer()
{
    return m_buffer.data();
}

void SphereData::setData(QVector<SphereVBOData> data)
{
    QByteArray ba;
    ba.resize(data.size() * sizeof(SphereVBOData));
    SphereVBOData *posData = reinterpret_cast<SphereVBOData*>(ba.data());
    // TODO can we just set the address here? Instead of copying.
    for(const SphereVBOData &pos : data) {
        *posData++ = pos;
    }
    m_buffer->setData(ba);
    m_count = data.count();
    emit countChanged(m_count);
}

void SphereData::setPositions(QVector<QVector3D> positions, QVector3D color, float scale)
{
    QByteArray ba;
    ba.resize(positions.size() * sizeof(SphereVBOData));
    SphereVBOData *vboData = reinterpret_cast<SphereVBOData *>(ba.data());
    for(int i=0; i<positions.size(); i++) {
        SphereVBOData &vbo = vboData[i];
        vbo.position = positions[i];
        // vbo.color = color;
        vbo.scale = scale;
    }
    m_buffer->setData(ba);
    m_count = positions.count();
    emit countChanged(m_count);
}

int SphereData::count() const
{
    return m_count;
}
