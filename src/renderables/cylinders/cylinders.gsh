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
out vec3 axis;
out vec3 U;
out vec3 V;
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

    axis = normalize(cp_normalMatrix * deltaNormalized);

    float r1 = vs_radius1[0];
    float r2 = vs_radius1[0]; // TODO use both radii

    vec3 center = (v1 + v2) * 0.5;
    vec3 cam_dir = (cp_cameraPosition - center);

    vec3 right = cross(cam_dir, deltaNormalized);
    right = normalize(right);

    vec3 outward = -cross(right, deltaNormalized);
    outward = normalize(outward);

    U = normalize(cp_normalMatrix * outward);
    V = normalize(cp_normalMatrix * right);

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
        float rtmp = r1;
        r1 = r2;
        r2 = rtmp;
    }

    vec4 vertices[12];
    vertices[ 0] = MV*vec4(v1 - right*r1 - outward*r1, 1.0); // cap top left
    vertices[ 1] = MV*vec4(v1 + right*r1 - outward*r1, 1.0);
    vertices[ 2] = MV*vec4(v1 - right*r1 + outward*r1, 1.0); // cap bottom left
    vertices[ 3] = MV*vec4(v1 + right*r1 + outward*r1, 1.0);

    vertices[ 4] = MV*vec4(v1 - right*r1 + outward*r1, 1.0); // main top left
    vertices[ 5] = MV*vec4(v1 + right*r1 + outward*r1, 1.0);
    vertices[ 6] = MV*vec4(v2 - right*r2 + outward*r2, 1.0); // main bottom left
    vertices[ 7] = MV*vec4(v2 + right*r2 + outward*r2, 1.0);

//    vertices[ 8] = MV*vec4(v2 - right*r2 - outward*r2, 1.0); // cap top left
//    vertices[ 9] = MV*vec4(v2 + right*r2 - outward*r2, 1.0);
//    vertices[10] = MV*vec4(v2 - right*r2 + outward*r2, 1.0); // cap bottom left
//    vertices[11] = MV*vec4(v2 + right*r2 + outward*r2, 1.0);

    vec2 texCoords[12];
    texCoords[ 0] = vec2(-1.0, -1.0);
    texCoords[ 1] = vec2(1.0, -1.0);
    texCoords[ 2] = vec2(-1.0, 1.0);
    texCoords[ 3] = vec2(1.0, 1.0);

    texCoords[ 4] = vec2(-1.0, 0.0);
    texCoords[ 5] = vec2(1.0, 0.0);
    texCoords[ 6] = vec2(-1.0, 0.0);
    texCoords[ 7] = vec2(1.0, 0.0);

//    texCoords[ 8] = vec2(-1.0, -1.0);
//    texCoords[ 9] = vec2(1.0, -1.0);
//    texCoords[10] = vec2(-1.0, 1.0);
//    texCoords[11] = vec2(1.0, 1.0);

    float radiuses[8];
    radiuses[0] = r1;
    radiuses[1] = r1;
    radiuses[2] = r1;
    radiuses[3] = r1;

    radiuses[4] = r1;
    radiuses[5] = r1;
    radiuses[6] = r2;
    radiuses[7] = r2;

    // top cap
    for(int i = 0; i < 8; i++) {
        vertexPosition = vertices[i].xyz / vertices[i].w;
        gl_Position = P*vertices[i];
        texCoord = texCoords[i];
//        radius = radiuses[i];
        radiusA = r1;
        radiusB = r2;
        EmitVertex();
    }

    EndPrimitive();

}





