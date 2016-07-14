attribute highp vec3 vertex1Position;
attribute highp vec3 vertex2Position;
attribute highp float radius1;
attribute highp float radius2;
attribute highp float sphereRadius1;
attribute highp float sphereRadius2;
attribute highp float vertexId;

varying highp vec3 vs_vertex1Position;
varying highp vec3 vs_vertex2Position;
varying highp float vs_sphereRadius1;
varying highp float vs_sphereRadius2;
varying highp float vs_radius1;
varying highp float vs_radius2;

varying highp vec2 texCoord;
varying highp vec3 modelViewPosition;
varying highp vec3 modelPosition;
varying highp vec3 worldPosition;
varying highp vec3 base;
varying highp vec3 end;
varying highp float radiusA;
varying highp float radiusB;
varying highp float da;
varying highp mat3 cylinderBasis;
varying highp vec3 cylinderDirection;
varying highp mat3 cylinderWorldBasis;

varying highp vec3 perpendicular;
varying highp vec3 biperpendicular;

vec3 makePerpendicular(vec3 v) {
    if(v.x == 0.0 && v.y == 0.0) {
        if(v.z == 0.0) {
            return vec3(0.0, 0.0, 0.0);
        }
        return vec3(0.0, 1.0, 0.0);
    }
    return vec3(-v.y, v.x, 0.0);
}

void main(void)
{
    vs_vertex1Position = vertex1Position;
    vs_vertex2Position = vertex2Position;
    vs_radius1 = radius1;
    vs_radius2 = radius2;
    vs_sphereRadius1 = sphereRadius1;
    vs_sphereRadius2 = sphereRadius2;

    highp vec3 v1 = vs_vertex1Position;
    highp vec3 v2 = vs_vertex2Position;

    highp vec3 mv1 = v1;
    highp vec3 mv2 = v2;

    highp vec3 delta = v2 - v1;
    highp vec3 deltaNormalized = normalize(delta);

    // define a unique perpendicular to the axis that can be used by
    // the texture coordinate in the fragment shader
    perpendicular = normalize(makePerpendicular(deltaNormalized));
    biperpendicular = normalize(cross(delta, perpendicular));

    radiusA = vs_radius1;
    radiusB = vs_radius2;

    highp float rmax = max(radiusA, radiusB);

    highp vec3 center = (mv1 + mv2) * 0.5;
    highp vec3 cam_dir = (cp_cameraPosition - center);

    highp vec3 right = cross(cam_dir, deltaNormalized);
    right = normalize(right);

    highp vec3 outward = cross(deltaNormalized, right);
    outward = normalize(outward);

    // multiplying with normal matrix is the same as multiplying
    // with the upper 3x3 part of the modelview matrix,
    // except that it doesn't change vector lengths
    // and preserves normals when the modelview matrix has
    // non-uniform scaling
    cylinderBasis = mat3((cp_modelViewMatrix * vec4(outward, 0.0)).xyz, // U
                         (cp_modelViewMatrix * vec4(right, 0.0)).xyz, // V
                         (cp_modelViewMatrix * vec4(deltaNormalized, 0.0)).xyz); // axis

    cylinderWorldBasis = mat3(normalize(outward), // U
                              normalize(right), // V
                              normalize(deltaNormalized)); // axis

    highp vec4 mvv1 = cp_modelViewMatrix * vec4(v1, 1.0);
    highp vec4 mvv2 = cp_modelViewMatrix * vec4(v2, 1.0);
    // NOTE: dividing by w here is typically not necessary
    // w is seldom modified by model view matrices
    base = mvv1.xyz / mvv1.w;
    end = mvv2.xyz / mvv2.w;

    highp vec3 cameraToV1 = cp_cameraPosition - mv1;
    highp vec3 cameraToV2 = cp_cameraPosition - mv2;
    highp float distanceToV1 = dot(cameraToV1, cameraToV1);
    highp float distanceToV2 = dot(cameraToV2, cameraToV2);

    highp float sign = 1.0;
    if(distanceToV1 > distanceToV2) {
        highp vec3 vtmp = v1;
        v1 = v2;
        v2 = vtmp;
        sign = -1.0; // keeps everything front face (in case of back face culling)
    }

    highp vec3 vertices[6];
    vertices[ 0] = v1 + sign*right*rmax - outward*rmax; // cap top left
    vertices[ 1] = v1 - sign*right*rmax - outward*rmax;
    vertices[ 2] = v1 + sign*right*rmax + outward*rmax; // cap bottom left
    vertices[ 3] = v1 - sign*right*rmax + outward*rmax;

    vertices[ 4] = v2 + sign*right*rmax + outward*rmax; // main bottom left
    vertices[ 5] = v2 - sign*right*rmax + outward*rmax;

    highp vec2 texCoords[6];
    texCoords[ 0] = vec2(-1.0, -1.0);
    texCoords[ 1] = vec2(1.0, -1.0);
    texCoords[ 2] = vec2(-1.0, 1.0);
    texCoords[ 3] = vec2(1.0, 1.0);

    texCoords[ 4] = vec2(-1.0, 0.0);
    texCoords[ 5] = vec2(1.0, 0.0);

    int i = int(vertexId);
    highp vec4 ppos = vec4(vertices[i], 1.0);
    modelPosition = ppos.xyz;
    modelViewPosition = (cp_modelViewMatrix * ppos).xyz;
    worldPosition = vertices[i];
    gl_Position = cp_modelViewProjectionMatrix*vec4(vertices[i], 1.0);
}
