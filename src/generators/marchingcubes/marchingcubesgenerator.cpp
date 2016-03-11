// File Name: marchingcubes.cpp
// Last Modified: 2014-04-13
// Author: Anders Hafreager (based on source code provided
// by Raghavendra Chandrashekara and Cory Bloyd: http://paulbourke.net/geometry/polygonise/marchingsource.cpp)
// Email: anderhaf@fys.uio.no
//
// Description: This is the implementation file for the MarchingCubes class.

#include "marchingcubesgenerator.h"
#include "marchingcubestables.h"
#include <cmath>
#include <QVector3D>
#include <QHash>
#include <QDebug>
#include <iostream>
#include <algorithm>
#include <QElapsedTimer>
#include <QColor>
#include "marchingcubes.h"

Cube MarchingCubesGenerator::createCube() {
    // See http://paulbourke.net/geometry/polygonise/ for figure
    Cube cube;

    cube.vertices.array.resize(8);
    cube.vertices.array[0] = &cube.vertices.v_000;
    cube.vertices.array[1] = &cube.vertices.v_010;
    cube.vertices.array[2] = &cube.vertices.v_110;
    cube.vertices.array[3] = &cube.vertices.v_100;
    cube.vertices.array[4] = &cube.vertices.v_001;
    cube.vertices.array[5] = &cube.vertices.v_011;
    cube.vertices.array[6] = &cube.vertices.v_111;
    cube.vertices.array[7] = &cube.vertices.v_101;

    // Create edges
    cube.edges.edge0.point1 = &cube.vertices.v_000; // Vertex 0
    cube.edges.edge0.point2 = &cube.vertices.v_010; // Vertex 1

    cube.edges.edge1.point1 = &cube.vertices.v_010; // Vertex 0
    cube.edges.edge1.point2 = &cube.vertices.v_110; // Vertex 2

    cube.edges.edge2.point1 = &cube.vertices.v_110; // Vertex 2
    cube.edges.edge2.point2 = &cube.vertices.v_100; // Vertex 3

    cube.edges.edge3.point1 = &cube.vertices.v_100; // Vertex 3
    cube.edges.edge3.point2 = &cube.vertices.v_000; // Vertex 0


    cube.edges.edge4.point1 = &cube.vertices.v_001; // Vertex 4
    cube.edges.edge4.point2 = &cube.vertices.v_011; // Vertex 5

    cube.edges.edge5.point1 = &cube.vertices.v_011; // Vertex 5
    cube.edges.edge5.point2 = &cube.vertices.v_111; // Vertex 6

    cube.edges.edge6.point1 = &cube.vertices.v_111; // Vertex 6
    cube.edges.edge6.point2 = &cube.vertices.v_101; // Vertex 7

    cube.edges.edge7.point1 = &cube.vertices.v_101; // Vertex 7
    cube.edges.edge7.point2 = &cube.vertices.v_001; // Vertex 4


    cube.edges.edge8.point1 = &cube.vertices.v_000; // Vertex 0
    cube.edges.edge8.point2 = &cube.vertices.v_001; // Vertex 4

    cube.edges.edge9.point1 = &cube.vertices.v_010; // Vertex 1
    cube.edges.edge9.point2 = &cube.vertices.v_011; // Vertex 5

    cube.edges.edge10.point1 = &cube.vertices.v_110; // Vertex 2
    cube.edges.edge10.point2 = &cube.vertices.v_111; // Vertex 6

    cube.edges.edge11.point1 = &cube.vertices.v_100; // Vertex 3
    cube.edges.edge11.point2 = &cube.vertices.v_101; // Vertex 7

    return cube;
}


function<float (const QVector3D point)> MarchingCubesGenerator::scalarFieldEvaluator() const
{
    return m_scalarFieldEvaluator;
}

void MarchingCubesGenerator::synchronize(MarchingCubes *marchingCubes)
{
    if(!marchingCubes) return;
    setThreshold(marchingCubes->threshold());
    setResolution(marchingCubes->resolution());
    setMinValues(marchingCubes->minValues());
    setMaxValues(marchingCubes->maxValues());
}

void MarchingCubesGenerator::setScalarFieldEvaluator(const function<float (const QVector3D point)> &scalarFieldEvaluator)
{
    m_scalarFieldEvaluator = scalarFieldEvaluator;
}

void MarchingCubesGenerator::setColorEvaluator(const function<QVector3D (const QVector3D point)> &colorEvaluator)
{
    m_colorEvaluator = colorEvaluator;
    m_hasColorEvaluator = true;
}

void MarchingCubesGenerator::setColor(QVector3D color)
{
    m_color = color;
}

void MarchingCubesGenerator::setColor(const QColor &color)
{
    m_color = QVector3D(color.redF(), color.greenF(), color.blueF());
}

void MarchingCubesGenerator::makeDirty()
{
    m_dirty = true;
}

QVector3D MarchingCubesGenerator::minValues() const
{
    return m_minValues;
}

void MarchingCubesGenerator::setMinValues(const QVector3D &minValues)
{
    if(m_minValues == minValues) return;
    m_minValues = minValues;
    makeDirty();
}

QVector3D MarchingCubesGenerator::maxValues() const
{
    return m_maxValues;
}

void MarchingCubesGenerator::setMaxValues(const QVector3D &maxValues)
{
    if(m_maxValues == maxValues) return;
    m_maxValues = maxValues;
    makeDirty();
}

QVector3D MarchingCubesGenerator::resolution() const
{
    return m_resolution;
}

void MarchingCubesGenerator::setResolution(const QVector3D &resolution)
{
    if(m_resolution == resolution) return;
    m_resolution = resolution;
    makeDirty();
}

QVector<TriangleCollectionVBOData> MarchingCubesGenerator::data() const
{
    return m_data;
}

QVector<Triangle> MarchingCubesGenerator::trianglesBack() const
{
    return m_trianglesBack;
}

QVector<Triangle> MarchingCubesGenerator::trianglesFront() const
{
    return m_trianglesFront;
}

bool MarchingCubesGenerator::getHasContinuousField() const
{
    return m_hasContinuousField;
}

void MarchingCubesGenerator::setHasContinuousField(bool hasContinuousField)
{
    m_hasContinuousField = hasContinuousField;
}

bool MarchingCubesGenerator::getHasColorEvaluator() const
{
    return m_hasColorEvaluator;
}

void MarchingCubesGenerator::setHasColorEvaluator(bool hasColorEvaluator)
{
    m_hasColorEvaluator = hasColorEvaluator;
}

void MarchingCubesGenerator::updateCube(Cube &cube, const QVector3D &minValues, const QVector3D &vertexIndices, const QVector3D &delta) {
    cube.vertices.v_000.coordinates[0] = double(minValues[0]) + vertexIndices[0]*delta[0];
    cube.vertices.v_000.coordinates[1] = double(minValues[1]) + vertexIndices[1]*delta[1];
    cube.vertices.v_000.coordinates[2] = double(minValues[2]) + vertexIndices[2]*delta[2];

    cube.vertices.v_001.coordinates[0] = double(minValues[0]) + vertexIndices[0]*delta[0];
    cube.vertices.v_001.coordinates[1] = double(minValues[1]) + vertexIndices[1]*delta[1];
    cube.vertices.v_001.coordinates[2] = double(minValues[2]) + (vertexIndices[2]+1.0)*delta[2];

    cube.vertices.v_011.coordinates[0] = double(minValues[0]) + vertexIndices[0]*delta[0];
    cube.vertices.v_011.coordinates[1] = double(minValues[1]) + (vertexIndices[1]+1.0)*delta[1];
    cube.vertices.v_011.coordinates[2] = double(minValues[2]) + (vertexIndices[2]+1.0)*delta[2];

    cube.vertices.v_010.coordinates[0] = double(minValues[0]) + vertexIndices[0]*delta[0];
    cube.vertices.v_010.coordinates[1] = double(minValues[1]) + (vertexIndices[1]+1.0)*delta[1];
    cube.vertices.v_010.coordinates[2] = double(minValues[2]) + vertexIndices[2]*delta[2];

    cube.vertices.v_110.coordinates[0] = double(minValues[0]) + (vertexIndices[0]+1.0)*delta[0];
    cube.vertices.v_110.coordinates[1] = double(minValues[1]) + (vertexIndices[1]+1.0)*delta[1];
    cube.vertices.v_110.coordinates[2] = double(minValues[2]) + vertexIndices[2]*delta[2];

    cube.vertices.v_111.coordinates[0] = double(minValues[0]) + (vertexIndices[0]+1.0)*delta[0];
    cube.vertices.v_111.coordinates[1] = double(minValues[1]) + (vertexIndices[1]+1.0)*delta[1];
    cube.vertices.v_111.coordinates[2] = double(minValues[2]) + (vertexIndices[2]+1.0)*delta[2];

    cube.vertices.v_101.coordinates[0] = double(minValues[0]) + (vertexIndices[0]+1.0)*delta[0];
    cube.vertices.v_101.coordinates[1] = double(minValues[1]) + vertexIndices[1]*delta[1];
    cube.vertices.v_101.coordinates[2] = double(minValues[2]) + (vertexIndices[2]+1.0)*delta[2];

    cube.vertices.v_100.coordinates[0] = double(minValues[0]) + (vertexIndices[0]+1.0)*delta[0];
    cube.vertices.v_100.coordinates[1] = double(minValues[1]) + vertexIndices[1]*delta[1];
    cube.vertices.v_100.coordinates[2] = double(minValues[2]) + vertexIndices[2]*delta[2];

    cube.vertices.v_000.value = m_scalarFieldEvaluator(cube.vertices.v_000.coordinates);
    cube.vertices.v_001.value = m_scalarFieldEvaluator(cube.vertices.v_001.coordinates);
    cube.vertices.v_011.value = m_scalarFieldEvaluator(cube.vertices.v_011.coordinates);
    cube.vertices.v_010.value = m_scalarFieldEvaluator(cube.vertices.v_010.coordinates);
    cube.vertices.v_110.value = m_scalarFieldEvaluator(cube.vertices.v_110.coordinates);
    cube.vertices.v_111.value = m_scalarFieldEvaluator(cube.vertices.v_111.coordinates);
    cube.vertices.v_101.value = m_scalarFieldEvaluator(cube.vertices.v_101.coordinates);
    cube.vertices.v_100.value = m_scalarFieldEvaluator(cube.vertices.v_100.coordinates);
}

MarchingCubesGenerator::MarchingCubesGenerator()
{
    m_resolution = QVector3D(10, 10, 10); // TODO: REMOVE???

}

MarchingCubesGenerator::~MarchingCubesGenerator()
{
    deleteSurface();
}

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/
Vertex MarchingCubesGenerator::vertexInterp(Vertex &p1, Vertex &p2)
{
    Vertex point;

    if (fabs(m_threshold-p1.value) < 0.00001)
        return(p1);
    if (fabs(m_threshold-p2.value) < 0.00001)
        return(p2);
    if (fabs(p1.value-p2.value) < 0.00001)
        return(p1);
    float mu = (m_threshold - p1.value) / (p2.value - p1.value);
    point.coordinates = p1.coordinates + mu*(p2.coordinates-p1.coordinates);
    point.value = p1.value + mu*(p2.value-p1.value);

    return(point);
}

int MarchingCubesGenerator::polygoniseTri(const Cube &cube, TRIANGLE *triangles, int v0,int v1,int v2,int v3)
{
    int ntri = 0;

    /*
      Determine which of the 16 cases we have given which vertices
      are above or below the isosurface
   */
    int triindex = 0;
    if (cube.vertices.array[v0]->value < m_threshold) triindex |= 1;
    if (cube.vertices.array[v1]->value < m_threshold) triindex |= 2;
    if (cube.vertices.array[v2]->value < m_threshold) triindex |= 4;
    if (cube.vertices.array[v3]->value < m_threshold) triindex |= 8;

    /* Form the vertices of the triangles for each case */
    switch (triindex) {
    case 0x00:
    case 0x0F:
        break;
    case 0x0E:
    case 0x01:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v1]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v2]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v3]);
        ntri++;
        break;
    case 0x0D:
    case 0x02:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v0]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v3]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v2]);
        ntri++;
        break;
    case 0x0C:
    case 0x03:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v3]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v2]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v3]);
        ntri++;
        triangles[1].points[0] = triangles[0].points[2];
        triangles[1].points[1] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v2]);
        triangles[1].points[2] = triangles[0].points[1];
        ntri++;
        break;
    case 0x0B:
    case 0x04:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v2], *cube.vertices.array[v0]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v2], *cube.vertices.array[v1]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v2], *cube.vertices.array[v3]);
        ntri++;
        break;
    case 0x0A:
    case 0x05:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v1]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v2], *cube.vertices.array[v3]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v3]);
        ntri++;
        triangles[1].points[0] = triangles[0].points[0];
        triangles[1].points[1] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v2]);
        triangles[1].points[2] = triangles[0].points[1];
        ntri++;
        break;
    case 0x09:
    case 0x06:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v1]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v1], *cube.vertices.array[v3]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v2], *cube.vertices.array[v3]);
        ntri++;
        triangles[1].points[0] = triangles[0].points[0];
        triangles[1].points[1] = vertexInterp(*cube.vertices.array[v0], *cube.vertices.array[v2]);
        triangles[1].points[2] = triangles[0].points[2];
        ntri++;
        break;
    case 0x07:
    case 0x08:
        triangles[0].points[0] = vertexInterp(*cube.vertices.array[v3], *cube.vertices.array[v0]);
        triangles[0].points[1] = vertexInterp(*cube.vertices.array[v3], *cube.vertices.array[v2]);
        triangles[0].points[2] = vertexInterp(*cube.vertices.array[v3], *cube.vertices.array[v1]);
        ntri++;
        break;
    }

    return(ntri);
}

void MarchingCubesGenerator::generateSurface2()
{
    if(!m_dirty) return;
    m_dirty = false;

    QElapsedTimer elapsed;
    elapsed.start();

    QVector3D delta = (m_maxValues - m_minValues) / m_resolution;

    // Generate isosurface.
    Cube cube = createCube();

    CubeVertices &cubeVertices = cube.vertices;
    CubeEdges &cubeEdges = cube.edges;

    QVector<TRIANGLE> triangles;
    triangles.resize(1e7);
    int triangleCount = 0;

    m_data.clear();

    for (unsigned int i = 0; i < m_resolution[0]; i++) {
        for (unsigned int j = 0; j < m_resolution[1]; j++) {
            for (unsigned int k = 0; k < m_resolution[2]; k++) {
                QVector3D indices(i,j,k);
                updateCube(cube, m_minValues, indices, delta);
                int newTriangleCount;
//                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 0,3,2,7);
//                triangleCount += newTriangleCount;

//                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 0,2,6,7);
//                triangleCount += newTriangleCount;

                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 0,4,6,7);
                triangleCount += newTriangleCount;

//                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 0,6,1,2);
//                triangleCount += newTriangleCount;

//                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 0,6,1,4);
//                triangleCount += newTriangleCount;

//                newTriangleCount = polygoniseTri(cube, &triangles[triangleCount], 5,6,1,4);
//                triangleCount += newTriangleCount;
            }
        }
    }

    int numberOfTriangles = triangleCount;
    triangles.resize(numberOfTriangles);
    m_data.resize(3*numberOfTriangles);
    m_trianglesFront.resize(numberOfTriangles);
    m_trianglesBack.resize(numberOfTriangles);
    int vertexCount = 0;
    triangleCount = 0;

    for(const TRIANGLE &triangle : triangles) {
        m_data[vertexCount+0].vertex = triangle.points[0].coordinates;
        m_data[vertexCount+1].vertex = triangle.points[1].coordinates;
        m_data[vertexCount+2].vertex = triangle.points[2].coordinates;
        m_trianglesFront[triangleCount].vertexIndices[0] = vertexCount+0;
        m_trianglesFront[triangleCount].vertexIndices[1] = vertexCount+1;
        m_trianglesFront[triangleCount].vertexIndices[2] = vertexCount+2;
        m_trianglesBack[triangleCount].vertexIndices[0] = vertexCount+0;
        m_trianglesBack[triangleCount].vertexIndices[1] = vertexCount+2;
        m_trianglesBack[triangleCount].vertexIndices[2] = vertexCount+1;
        if(m_hasColorEvaluator) {
            m_data[vertexCount+0].color = m_colorEvaluator(m_data[vertexCount+0].vertex);
            m_data[vertexCount+1].color = m_colorEvaluator(m_data[vertexCount+1].vertex);
            m_data[vertexCount+2].color = m_colorEvaluator(m_data[vertexCount+2].vertex);
        }

        vertexCount+= 3;
    }
    calculateNormals();

    qDebug() << "Marching cubes created with " << m_data.size() << " vertices, " << 2*m_trianglesFront.size() << " triangles in " << elapsed.elapsed() << " ms.";
}


void MarchingCubesGenerator::generateSurface()
{
    if(!m_dirty) return;
        m_dirty = false;

        QElapsedTimer elapsed;
        elapsed.start();

        deleteSurface();
        QVector3D delta = (m_maxValues - m_minValues) / m_resolution;

        // Generate isosurface.
        Cube cube = createCube();

        CubeVertices &cubeVertices = cube.vertices;
        CubeEdges &cubeEdges = cube.edges;

        for (unsigned int i = 0; i < m_resolution[0]; i++) {
            for (unsigned int j = 0; j < m_resolution[1]; j++) {
                for (unsigned int k = 0; k < m_resolution[2]; k++) {
                    QVector3D indices(i,j,k);
                    updateCube(cube, m_minValues, indices, delta);

                    // Calculate table lookup index from those
                    // vertices which are below the isolevel.
                    unsigned int tableIndex = 0;

                    if (cubeVertices.v_000.value < m_threshold) tableIndex |= 1;
                    if (cubeVertices.v_010.value < m_threshold) tableIndex |= 2;
                    if (cubeVertices.v_110.value < m_threshold) tableIndex |= 4;
                    if (cubeVertices.v_100.value < m_threshold) tableIndex |= 8;
                    if (cubeVertices.v_001.value < m_threshold) tableIndex |= 16;
                    if (cubeVertices.v_011.value < m_threshold) tableIndex |= 32;
                    if (cubeVertices.v_111.value < m_threshold) tableIndex |= 64;
                    if (cubeVertices.v_101.value < m_threshold) tableIndex |= 128;

                    // Now create a triangulation of the isosurface in this cell.
                    if (m_edgeTable[tableIndex] != 0) {
                        if (m_edgeTable[tableIndex] & 8) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge3);
                            unsigned int id = getEdgeId(i, j, k, 3);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 1) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge0);
                            unsigned int id = getEdgeId(i, j, k, 0);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 256) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge8);
                            unsigned int id = getEdgeId(i, j, k, 8);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 4) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge2);
                            unsigned int id = getEdgeId(i, j, k, 2);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }
                        if (m_edgeTable[tableIndex] & 2048) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge11);
                            unsigned int id = getEdgeId(i, j, k, 11);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 2) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge1);
                            unsigned int id = getEdgeId(i, j, k, 1);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }
                        if (m_edgeTable[tableIndex] & 512) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge9);
                            unsigned int id = getEdgeId(i, j, k, 9);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 16) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge4);
                            unsigned int id = getEdgeId(i, j, k, 4);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }
                        if (m_edgeTable[tableIndex] & 128) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge7);
                            unsigned int id = getEdgeId(i, j, k, 7);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 1024) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge10);
                            unsigned int id = getEdgeId(i, j, k, 10);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        if (m_edgeTable[tableIndex] & 64) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge6);
                            unsigned int id = getEdgeId(i, j, k, 6);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }
                        if (m_edgeTable[tableIndex] & 32) {
                            QVector3D intersectionPoint = calculateIntersection(cubeEdges.edge5);
                            unsigned int id = getEdgeId(i, j, k, 5);
                            m_edgeMap.insert(VertexMap::value_type(id, intersectionPoint));
                        }

                        for (unsigned int l = 0; m_triangleTable[tableIndex][l] != -1; l += 3) {
                            unsigned int vertex0Id = getEdgeId(i, j, k, m_triangleTable[tableIndex][l]);
                            unsigned int vertex1Id = getEdgeId(i, j, k, m_triangleTable[tableIndex][l+1]);
                            unsigned int vertex2Id = getEdgeId(i, j, k, m_triangleTable[tableIndex][l+2]);
                            m_trianglesFront.push_back(Triangle(vertex0Id, vertex1Id, vertex2Id));
                        }
                    }
                }
            }
        }

        // Add all these triangles sequentially in memory and note the mapping between edgeID and
        // index in the sequential index
        m_data.resize(m_edgeMap.size());
        std::map<unsigned int, unsigned int> vertexMap;

        unsigned int counter = 0;
        for(auto it=m_edgeMap.begin(); it!=m_edgeMap.end(); it++) {
            QVector3D &vertex = it->second;
            unsigned int index = it->first;
            // Map this vertex in the map to the index of the sequential vector
            vertexMap[index] = counter;
            m_data[counter].vertex = vertex;
            if(!m_hasColorEvaluator) m_data[counter].color = m_color;
            else m_data[counter].color = m_colorEvaluator(vertex);
            counter++;
        }

        // Update triangle list with the new indices
        m_trianglesBack.resize(m_trianglesFront.size());
        for(unsigned int i=0; i<m_trianglesFront.size(); i++) {
            Triangle &triangleFront = m_trianglesFront[i];
            Triangle &triangleBack = m_trianglesBack[i];

            // Switch the indices from edgeID ordering to sequential ordering
            triangleFront.vertexIndices[0] = vertexMap[triangleFront.vertexIndices[0]];
            triangleFront.vertexIndices[1] = vertexMap[triangleFront.vertexIndices[1]];
            triangleFront.vertexIndices[2] = vertexMap[triangleFront.vertexIndices[2]];

            // The back triangles are just rendered with opposite order
            triangleBack.vertexIndices[0] = triangleFront.vertexIndices[2];
            triangleBack.vertexIndices[1] = triangleFront.vertexIndices[1];
            triangleBack.vertexIndices[2] = triangleFront.vertexIndices[0];
        }

        qDebug() << "Marching cubes created with " << m_data.size() << " vertices, " << 2*m_trianglesFront.size() << " trianglessss in " << elapsed.elapsed() << " ms.";

        elapsed.restart();
        calculateNormals();

        m_edgeMap.clear();
        vertexMap.clear();
}


void MarchingCubesGenerator::setThreshold(float threshold)
{
    if(m_threshold == threshold) return;
    m_threshold = threshold;
    m_dirty = true;
}

void MarchingCubesGenerator::deleteSurface()
{
    m_data.clear();
    m_trianglesFront.clear();
    m_trianglesBack.clear();
}

unsigned int MarchingCubesGenerator::getEdgeId(unsigned int i, unsigned int j, unsigned int k, unsigned int nEdgeNo)
{
    switch (nEdgeNo) {
    case 0:
        return getVertexID(i, j, k) + 1;
    case 1:
        return getVertexID(i, j + 1, k);
    case 2:
        return getVertexID(i + 1, j, k) + 1;
    case 3:
        return getVertexID(i, j, k);
    case 4:
        return getVertexID(i, j, k + 1) + 1;
    case 5:
        return getVertexID(i, j + 1, k + 1);
    case 6:
        return getVertexID(i + 1, j, k + 1) + 1;
    case 7:
        return getVertexID(i, j, k + 1);
    case 8:
        return getVertexID(i, j, k) + 2;
    case 9:
        return getVertexID(i, j + 1, k) + 2;
    case 10:
        return getVertexID(i + 1, j + 1, k) + 2;
    case 11:
        return getVertexID(i + 1, j, k) + 2;
    default:
        qDebug() << "Invalid edge ID, something is clearly wrong.";
        exit(1);
    }
}

unsigned int MarchingCubesGenerator::getVertexID(unsigned int i, unsigned int j, unsigned int k)
{
    return 3*(k*(m_resolution[1]+1)*(m_resolution[0]+1) + j*(m_resolution[0]+1) + i);
}

QVector3D MarchingCubesGenerator::calculateIntersection(Edge &edge)
{
    float linearDistance = (m_threshold - edge.point1->value) / (edge.point2->value - edge.point1->value);
    QVector3D intersectionPoint;

    intersectionPoint[0] = edge.point1->x() + linearDistance*(edge.point2->x() - edge.point1->x());
    intersectionPoint[1] = edge.point1->y() + linearDistance*(edge.point2->y() - edge.point1->y());
    intersectionPoint[2] = edge.point1->z() + linearDistance*(edge.point2->z() - edge.point1->z());

    return intersectionPoint;
}

void MarchingCubesGenerator::calculateNormal(TriangleCollectionVBOData &data) {
    // Numerical differentiation with the two point formula:
    // f' ≈ ( f(x+h) - f(x-h) ) / 2h
    static const float h = 0.01;
    static const float oneOverTwoH = 1.0 / (2*h);

    float fPlusX = m_scalarFieldEvaluator(QVector3D(data.vertex.x() + h, data.vertex.y(), data.vertex.z()));
    float fMinusX = m_scalarFieldEvaluator(QVector3D(data.vertex.x() - h, data.vertex.y(), data.vertex.z()));

    float fPlusY = m_scalarFieldEvaluator(QVector3D(data.vertex.x(), data.vertex.y() + h, data.vertex.z()));
    float fMinusY = m_scalarFieldEvaluator(QVector3D(data.vertex.x(), data.vertex.y() - h, data.vertex.z()));

    float fPlusZ = m_scalarFieldEvaluator(QVector3D(data.vertex.x(), data.vertex.y(), data.vertex.z() + h));
    float fMinusZ = m_scalarFieldEvaluator(QVector3D(data.vertex.x(), data.vertex.y(), data.vertex.z() - h));

    data.normal[0] = (fPlusX - fMinusX) * oneOverTwoH;
    data.normal[1] = (fPlusY - fMinusY) * oneOverTwoH;
    data.normal[2] = (fPlusZ - fMinusZ) * oneOverTwoH;
    data.normal.normalize();
}

void MarchingCubesGenerator::calculateNormals()
{
    if(m_hasContinuousField) {
        for (TriangleCollectionVBOData &data : m_data) {
            calculateNormal(data);
        }
    } else {
        for(Triangle &triangle : m_trianglesFront) {
            TriangleCollectionVBOData &v0 = m_data[triangle.vertexIndices[0]];
            TriangleCollectionVBOData &v1 = m_data[triangle.vertexIndices[1]];
            TriangleCollectionVBOData &v2 = m_data[triangle.vertexIndices[2]];
            QVector3D p1 = v2.vertex - v0.vertex;
            QVector3D p2 = v1.vertex - v0.vertex;
            QVector3D crossProduct = QVector3D::crossProduct(p1, p2);
            QVector3D normal = crossProduct;

            v0.normal += normal;
            v1.normal += normal;
            v2.normal += normal;
        }

        for(TriangleCollectionVBOData &data : m_data) {
            data.normal.normalize();
        }
    }
}
