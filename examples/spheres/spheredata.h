#ifndef SPHEREDATA_H
#define SPHEREDATA_H

#include <QObject>
#include <Qt3DRender/QBuffer>

class SphereData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QBuffer* buffer READ buffer CONSTANT)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit SphereData(QObject *parent = 0);

    Qt3DRender::QBuffer* buffer();
    void setPositions(QVector<QVector3D> positions);
    int count() const;

signals:

    void countChanged(int count);

public slots:

private:
    Qt3DRender::QBuffer m_buffer;
    int m_count;
};

#endif // SPHEREDATA_H
