layout( points ) in;
layout( triangle_strip, max_vertices = 12 ) out;
//uniform float radius;
in vec3 vs_vertex1Position[1];
in vec3 vs_vertex2Position[1];
in float vs_radius1[1];
in float vs_radius2[1];

out vec2 texCoord;
out vec3 vertexPosition;
out vec3 cylinderDirection;
out mat3 cylinderBasis;
out float da;
out float radius;
out float radiusA;
out float radiusB;
out vec3 base;
out vec3 end_cyl;

void main(void) {
    mat4 MV = cp_modelViewMatrix;
    mat4 MVP = cp_modelViewProjectionMatrix;
    mat4 P = cp_projectionMatrix;

    vec3 v1 = vs_vertex1Position[0];
    vec3 v2 = vs_vertex2Position[0];

    vec3 delta = v2 - v1;
    vec3 deltaNormalized = normalize(delta);

    radiusA = vs_radius1[0];
    radiusB = vs_radius2[0];

    float rmax = max(radiusA, radiusB);

    vec3 center = (v1 + v2) * 0.5;
    vec3 cam_dir = (cp_cameraPosition - center);

    vec3 right = cross(cam_dir, deltaNormalized);
    right = normalize(right);

    vec3 outward = cross(deltaNormalized, right);
    outward = normalize(outward);

    cylinderBasis = mat3(normalize(cp_normalMatrix * outward), // U
                         normalize(cp_normalMatrix * right), // V
                         normalize(cp_normalMatrix * deltaNormalized)); // axis

    vec4 mvv1 = MV * vec4(v1, 1.0);
    vec4 mvv2 = MV * vec4(v2, 1.0);
    // NOTE: dividing by w here is typically not necessary
    // w is seldom modified by model view matrices
    base = mvv1.xyz / mvv1.w;
    end_cyl = mvv2.xyz / mvv2.w;

    vec3 cameraToV1 = cp_cameraPosition - v1;
    vec3 cameraToV2 = cp_cameraPosition - v2;
    float distanceToV1 = dot(cameraToV1, cameraToV1);
    float distanceToV2 = dot(cameraToV2, cameraToV2);

    if(distanceToV1 > distanceToV2) {
        vec3 vtmp = v1;
        v1 = v2;
        v2 = vtmp;
    }

    vec4 vertices[6];
    vertices[ 0] = MV*vec4(v1 - right*rmax - outward*rmax, 1.0); // cap top left
    vertices[ 1] = MV*vec4(v1 + right*rmax - outward*rmax, 1.0);
    vertices[ 2] = MV*vec4(v1 - right*rmax + outward*rmax, 1.0); // cap bottom left
    vertices[ 3] = MV*vec4(v1 + right*rmax + outward*rmax, 1.0);

    vertices[ 4] = MV*vec4(v2 - right*rmax + outward*rmax, 1.0); // main bottom left
    vertices[ 5] = MV*vec4(v2 + right*rmax + outward*rmax, 1.0);

    vec2 texCoords[6];
    texCoords[ 0] = vec2(-1.0, -1.0);
    texCoords[ 1] = vec2(1.0, -1.0);
    texCoords[ 2] = vec2(-1.0, 1.0);
    texCoords[ 3] = vec2(1.0, 1.0);

    texCoords[ 4] = vec2(-1.0, 0.0);
    texCoords[ 5] = vec2(1.0, 0.0);

    // top cap
    for(int i = 0; i < 6; i++) {
        vertexPosition = vertices[i].xyz / vertices[i].w;
        gl_Position = P*vertices[i];
        texCoord = texCoords[i];
        EmitVertex();
    }

    EndPrimitive();

}





