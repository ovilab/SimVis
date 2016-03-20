#include "functionsurfacedata.h"

#include <QVector3D>

using Qt3DRender::QBuffer;

FunctionSurface::FunctionSurface(QNode *parent)
    : QNode(parent)
    , m_buffer(new QBuffer(QBuffer::VertexBuffer, this))
{
}

Qt3DRender::QBuffer *FunctionSurface::buffer()
{
    return m_buffer.data();
}

void FunctionSurface::setPositions(QVector<QVector3D> positions)
{
    QByteArray ba;
    ba.resize(positions.size() * sizeof(QVector3D));
    QVector3D *posData = reinterpret_cast<QVector3D *>(ba.data());
    for(const QVector3D &pos : positions) {
        *posData = pos;
        ++posData;
    }
    m_buffer->setData(ba);
    m_count = positions.count();
    emit countChanged(m_count);
}

int FunctionSurface::count() const
{
    return m_count;
}
