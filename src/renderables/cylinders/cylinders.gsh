layout( points ) in;
layout( triangle_strip, max_vertices = 12 ) out;
uniform float radius;
in vec3 vs_vertex1Position[1];
in vec3 vs_vertex2Position[1];
in float vs_radius1[1];
in float vs_radius2[1];

out vec2 texCoord;
out vec3 vertexPosition;
out vec3 cylinderDirection;
out float da;

void main(void) {
    vec3 v1 = vs_vertex1Position[0];
    vec3 v2 = vs_vertex2Position[0];

    vec3 delta = v2 - v1;
    vec3 deltaNormalized = normalize(delta);

    float r1 = vs_radius1[0];
    float r2 = vs_radius2[0];

    vec3 right = cross(cp_viewVector, deltaNormalized);
    right = normalize(right);

    vec3 outward = cross(right, deltaNormalized);
    outward = normalize(outward);

    mat4 MV = cp_modelViewMatrix;
    mat4 MVP = cp_modelViewProjectionMatrix;
    mat4 P = cp_projectionMatrix;

    vec4 vertices[12];
    vertices[ 0] = MV*vec4(v1 - right*r1 - outward*r1, 1.0); // cap top left
    vertices[ 1] = MV*vec4(v1 + right*r1 - outward*r1, 1.0);
    vertices[ 2] = MV*vec4(v1 - right*r1 + outward*r1, 1.0); // cap bottom left
    vertices[ 3] = MV*vec4(v1 + right*r1 + outward*r1, 1.0);

    vertices[ 4] = MV*vec4(v1 - right*r1, 1.0); // main top left
    vertices[ 5] = MV*vec4(v1 + right*r1, 1.0);
    vertices[ 6] = MV*vec4(v2 - right*r2, 1.0); // main bottom left
    vertices[ 7] = MV*vec4(v2 + right*r2, 1.0);

    vertices[ 8] = MV*vec4(v2 - right*r2 - outward*r2, 1.0); // cap top left
    vertices[ 9] = MV*vec4(v2 + right*r2 - outward*r2, 1.0);
    vertices[10] = MV*vec4(v2 - right*r2 + outward*r2, 1.0); // cap bottom left
    vertices[11] = MV*vec4(v2 + right*r2 + outward*r2, 1.0);

    vec2 texCoords[12];
    texCoords[ 0] = vec2(-1.0, -1.0);
    texCoords[ 1] = vec2(1.0, -1.0);
    texCoords[ 2] = vec2(-1.0, 1.0);
    texCoords[ 3] = vec2(1.0, 1.0);

    texCoords[ 4] = vec2(-1.0, 0.0);
    texCoords[ 5] = vec2(1.0, 0.0);
    texCoords[ 6] = vec2(-1.0, 0.0);
    texCoords[ 7] = vec2(1.0, 0.0);

    texCoords[ 8] = vec2(-1.0, -1.0);
    texCoords[ 9] = vec2(1.0, -1.0);
    texCoords[10] = vec2(-1.0, 1.0);
    texCoords[11] = vec2(1.0, 1.0);

    // top cap
    for(int i = 0; i < 12; i++) {
        vertexPosition = vertices[i].xyz / vertices[i].w;
        gl_Position = P*vertices[i];
        texCoord = texCoords[i];
        EmitVertex();
    }

    EndPrimitive();

}





