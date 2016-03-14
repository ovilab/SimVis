#include "spheredata.h"

#include <QVector3D>

using Qt3DRender::QBuffer;

SphereData::SphereData(QObject *parent)
    : QObject(parent)
{

}

Qt3DRender::QBuffer *SphereData::buffer()
{
    return &m_buffer;
}

void SphereData::setPositions(QVector<QVector3D> positions)
{
    QByteArray ba;
    ba.resize(positions.size() * sizeof(QVector3D));
    QVector3D *posData = reinterpret_cast<QVector3D *>(ba.data());
    for(const QVector3D &pos : positions) {
        *posData = pos;
        ++posData;
    }
    m_buffer.setData(ba);
    m_count = positions.count();
    emit countChanged(m_count);
}

int SphereData::count() const
{
    return m_count;
}
