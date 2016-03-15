#ifndef SPHEREDATA_H
#define SPHEREDATA_H

#include <QObject>
#include <Qt3DRender/QBuffer>
#include <Qt3DCore/QNode>

class SphereData : public Qt3DCore::QNode
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QBuffer* buffer READ buffer CONSTANT)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit SphereData(QNode *parent = 0);

    Qt3DRender::QBuffer* buffer();
    void setPositions(QVector<QVector3D> positions);
    int count() const;

signals:

    void countChanged(int count);

public slots:

private:
    QScopedPointer<Qt3DRender::QBuffer> m_buffer;
    int m_count = 0;

    QT3D_CLONEABLE(SphereData)
};

#endif // SPHEREDATA_H
