#include "marchingcubes.h"

MarchingCubes::MarchingCubes(QObject *parent) : QObject(parent)
{

}

float MarchingCubes::threshold() const
{
    return m_threshold;
}

QVector3D MarchingCubes::minValues() const
{
    return m_minValues;
}

QVector3D MarchingCubes::maxValues() const
{
    return m_maxValues;
}

QVector3D MarchingCubes::resolution() const
{
    return m_resolution;
}

void MarchingCubes::setThreshold(float threshold)
{
    if (m_threshold == threshold)
        return;

    m_threshold = threshold;
    emit thresholdChanged(threshold);
}

void MarchingCubes::setMinValues(QVector3D minValues)
{
    if (m_minValues == minValues)
        return;

    m_minValues = minValues;
    emit minValuesChanged(minValues);
}

void MarchingCubes::setMaxValues(QVector3D maxValues)
{
    if (m_maxValues == maxValues)
        return;

    m_maxValues = maxValues;
    emit maxValuesChanged(maxValues);
}

void MarchingCubes::setResolution(QVector3D resolution)
{
    if (m_resolution == resolution)
        return;

    m_resolution = resolution;
    emit resolutionChanged(resolution);
}
