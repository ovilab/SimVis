/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "instancebuffer.h"

#include <QtGui/qvector3d.h>
#include <cmath>
static const int rowCount = 2;
static const int colCount = 100;
static const int sliceCount = 100;

static const int maxInstanceCount = rowCount * colCount * sliceCount;

InstanceBuffer::InstanceBuffer(Qt3DCore::QNode *parent)
    : Qt3DRender::QBuffer(QBuffer::VertexBuffer, parent)
    , m_instanceCount(maxInstanceCount)
{
    setFactor(1.0);
}

int InstanceBuffer::instanceCount() const
{
    return m_instanceCount;
}

float InstanceBuffer::factor() const
{
    return m_factor;
}

void InstanceBuffer::setInstanceCount(int instanceCount)
{
    if (m_instanceCount == instanceCount)
        return;

    m_instanceCount = instanceCount;
    emit instanceCountChanged(instanceCount);
}

void InstanceBuffer::setFactor(float factor)
{
    if (m_factor == factor)
        return;

    m_factor = factor;

    // Create some per instance data - position of each instance
    QByteArray ba;
    ba.resize(maxInstanceCount * sizeof(QVector3D));

    QVector3D *posData = reinterpret_cast<QVector3D *>(ba.data());
    for (int k = 0; k < sliceCount; ++k) {
        const float z = float(k);
        for (int j = 0; j < rowCount; ++j) {
            const float y = float(j);
            for (int i = 0; i < colCount; ++i) {
                const float x = float(i);
                const QVector3D pos(x, sin(0.1*x + 0.5 * factor) + sin(0.1*z + 0.2*factor) + y, z);
                *posData = pos;
                ++posData;
            }
        }
    }

    // Put the data into the buffer
    setData(ba);

    emit factorChanged(factor);
}

