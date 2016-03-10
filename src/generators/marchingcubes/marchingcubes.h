#ifndef MARCHINGCUBES_H
#define MARCHINGCUBES_H

#include <QObject>
#include <QVector3D>
class MarchingCubes : public QObject
{
    Q_OBJECT
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(QVector3D minValues READ minValues WRITE setMinValues NOTIFY minValuesChanged)
    Q_PROPERTY(QVector3D maxValues READ maxValues WRITE setMaxValues NOTIFY maxValuesChanged)
    Q_PROPERTY(QVector3D resolution READ resolution WRITE setResolution NOTIFY resolutionChanged)
public:
    explicit MarchingCubes(QObject *parent = 0);
    float threshold() const;
    QVector3D minValues() const;
    QVector3D maxValues() const;
    QVector3D resolution() const;

signals:
    void thresholdChanged(float threshold);
    void minValuesChanged(QVector3D minValues);
    void maxValuesChanged(QVector3D maxValues);
    void resolutionChanged(QVector3D resolution);

public slots:
    void setThreshold(float threshold);
    void setMinValues(QVector3D minValues);
    void setMaxValues(QVector3D maxValues);
    void setResolution(QVector3D resolution);

private:
    float m_threshold = 0;
    QVector3D m_minValues;
    QVector3D m_maxValues;
    QVector3D m_resolution;
};

#endif // MARCHINGCUBES_H
