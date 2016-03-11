uniform vec3 delta;

struct Vertex {
    vec3 coordinates;
    float value;
};

struct Edge {
    Vertex point1;
    Vertex point2;
};

struct CubeEdges {
    // See http://paulbourke.net/geometry/polygonise/ for figure
    Edge edge0;
    Edge edge1;
    Edge edge2;
    Edge edge3;
    Edge edge4;
    Edge edge5;
    Edge edge6;
    Edge edge7;
    Edge edge8;
    Edge edge9;
    Edge edge10;
    Edge edge11;
};

struct CubeVertices {
    // See http://paulbourke.net/geometry/polygonise/ for figure
    vec3 v_000; // Corresponds to vertex 0
    vec3 v_001; // Corresponds to vertex 4
    vec3 v_011; // Corresponds to vertex 5
    vec3 v_010; // Corresponds to vertex 1
    vec3 v_110; // Corresponds to vertex 2
    vec3 v_111; // Corresponds to vertex 6
    vec3 v_101; // Corresponds to vertex 7
    vec3 v_100; // Corresponds to vertex 3
};

struct Cube {
    CubeVertices vertices;
    CubeEdges edges;
};

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

in vec3 vs_position[1];

out vec3 position;

void main(void) {
    vec3 v_000 = vs_position[0];                        // Corresponds to vertex 0
    vec3 v_001 = v_000 + vec3(0.0, 0.0, delta.z);       // Corresponds to vertex 4
    vec3 v_011 = v_000 + vec3(0.0, delta.yz);           // Corresponds to vertex 5
    vec3 v_010 = v_000 + vec3(0.0, delta.y, 0.0);       // Corresponds to vertex 1
    vec3 v_110 = v_000 + vec3(delta.xy, 0.0);           // Corresponds to vertex 2
    vec3 v_111 = v_000 + delta;                         // Corresponds to vertex 6
    vec3 v_101 = v_000 + vec3(delta.x, 0.0, delta.z);   // Corresponds to vertex 7
    vec3 v_100 = v_000 + vec3(delta.x, 0.0, 0.0);       // Corresponds to vertex 3

    vec4 vertex = cp_modelViewMatrix * (vec4(vs_position[0], 1.0) + displacement);


    vec4 displacement = vec4(0.0,0.0,0.0,0.0);
    sphereId = vs_sphereId[0];
#ifdef ADDPERIODICCOPIES
    int x = gl_InvocationID % 3 - 1;
    int y = (gl_InvocationID/3)%3-1;
    int z = (gl_InvocationID/9)-1;
    displacement = vec4(systemSize.x*x, systemSize.y*y, systemSize.z*z, 0.0);
    vec4 vertex = cp_modelViewMatrix * (vec4(vs_position[0], 1.0) + displacement);
    vec4 pos = gl_in[0].gl_Position + cp_modelViewProjectionMatrix * displacement;
#else
    vec4 pos = gl_in[0].gl_Position;
#endif

    bool addPoint = true;
#ifdef SLICE
    addPoint = slice_vectorIsInside(vs_position[0] + displacement.xyz);
#endif
    if(addPoint) {
        position = vs_position[0] + displacement.xyz;
        float scale = vs_scale[0];
        color = vs_color[0];

        gl_Position = pos + cp_projectionMatrix*vec4(-scale, -scale, 0.0, 0.0);
        texCoord = vec2(-1.0, -1.0);
        EmitVertex();
        gl_Position = pos + cp_projectionMatrix*vec4(-scale, scale, 0.0, 0.0);
        texCoord = vec2(-1.0, 1.0);
        EmitVertex();
        gl_Position = pos + cp_projectionMatrix*vec4(scale, -scale, 0.0, 0.0);
        texCoord = vec2(1.0, -1.0);
        EmitVertex();
        gl_Position = pos + cp_projectionMatrix*vec4(scale, scale, 0.0, 0.0);
        texCoord = vec2(1.0, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}
