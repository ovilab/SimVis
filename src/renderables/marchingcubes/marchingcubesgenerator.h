#ifndef MARCHINGCUBESGENERATOR_H
#define MARCHINGCUBESGENERATOR_H

// File Name: marchingcubes.h
// Last Modified: 2014-04-13
// Author: Anders Hafreager (basesd on source code
// provided by Raghavendra Chandrashekara)
// Email: anderhaf@fys.uio.no
//
// Description: This is the interface file for the MarchingCubes class.
// MarchingCubes can be used to construct an isosurface from a scalar
// field.

#include <map>
#include <array>
#include <QVector>
#include <functional>
#include <QVector3D>
#include <QPair>

using std::function;

struct POINT3DID {
    unsigned int newID;
    float x, y, z;
};

typedef std::map<unsigned int, POINT3DID> ID2POINT3DID;

struct Triangle {
    unsigned int pointID[3];
};

typedef QVector<Triangle> TRIANGLEVECTOR;

class MarchingCubesGenerator {
protected:
    float m_isoValue = 0.0;
    bool m_validSurface = false;
    QVector<QPair<unsigned int> > m_edgePoints;
    function<float(float x, float y, float z)> m_scalarFieldEvaluator;
private:
    void createEdgePointTable();
public:
    MarchingCubesGenerator(function<float(float x, float y, float z)> scalarFieldEvaluator, float isoValue = 0.0);
    ~MarchingCubesGenerator();

    // Generates the isosurface from the scalar field contained in scalarField array
    void generateSurface(QVector3D minValues, QVector3D maxValues, QVector3D numberOfCells);

    // Returns true if a valid surface has been generated.
    bool isSurfaceValid();

    // Deletes the isosurface.
    void deleteSurface();

    // The number of vertices which make up the isosurface.
    unsigned int m_nVertices;

    // The vertices which make up the isosurface.
    QArray<QVector3D> m_ppt3dVertices;

    // The number of triangles which make up the isosurface.
    unsigned int m_nTriangles;

    // The indices of the vertices which make up the triangles.
    QArray<std::array<unsigned int, 3> > m_piTriangleIndices;

    // The number of normals.
    unsigned int m_nNormals;

    // The normals.
    QArray<QVector3D> m_pvec3dNormals;

    // List of POINT3Ds which form the isosurface.
    ID2POINT3DID m_i2pt3idVertices;

    // List of TRIANGLES which form the triangulation of the isosurface.
    TRIANGLEVECTOR m_trivecTriangles;

    // Returns the edge ID.
    unsigned int getEdgeID(unsigned int nX, unsigned int nY, unsigned int nZ, unsigned int nEdgeNo);

    // Returns the vertex ID.
    unsigned int getVertexID(unsigned int nX, unsigned int nY, unsigned int nZ);

    // Calculates the intersection point of the isosurface with an
    // edge.
    POINT3DID calculateIntersection(const QVector3D &point, const QVector3D &delta, unsigned int edgeNumber);

    // Interpolates between two grid points to produce the point at which
    // the isosurface intersects an edge.
    POINT3DID interpolate(float fX1, float fY1, float fZ1, float fX2, float fY2, float fZ2, T tVal1, T tVal2);

    // Renames vertices and triangles so that they can be accessed more
    // efficiently.
    void renameVerticesAndTriangles();

    // Calculates the normals.
    void calculateNormals();

    // Lookup tables used in the construction of the isosurface.
    static const unsigned int m_edgeTable[256];
    static const int m_triTable[256][16];
    static const float m_cubeTable[12][2][3];
};

#endif // MARCHINGCUBESGENERATOR_H
