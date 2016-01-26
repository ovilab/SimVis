// BEGIN noiseviewer.gsh
// Based on this ordering: https://scs.senecac.on.ca/~chris.szalwinski/archives/gam666.083/images/strip.gif
layout( points ) in;
layout( triangle_strip, max_vertices = 24 ) out;

uniform vec3 delta000;
uniform vec3 delta001;
uniform vec3 delta010;
uniform vec3 delta011;
uniform vec3 delta100;
uniform vec3 delta101;
uniform vec3 delta110;
uniform vec3 delta111;

uniform vec4 mvp_delta000;
uniform vec4 mvp_delta001;
uniform vec4 mvp_delta010;
uniform vec4 mvp_delta011;
uniform vec4 mvp_delta100;
uniform vec4 mvp_delta101;
uniform vec4 mvp_delta110;
uniform vec4 mvp_delta111;

in vec3 vs_vertexPosition[1];
out vec3 vertexPosition;
out vec3 normal;

void main(void) {
    vec3 pos = vs_vertexPosition[0];
    if(cos(pos.x)*sin(pos.y) > 0.75) {
        vec4 mvp_position = cp_modelViewProjectionMatrix*vec4(pos, 1.0);
        vec3 vertexPositions[8];
        vec4 mvp_positions[8];
        vertexPositions[0] = pos;
        mvp_positions[0] = mvp_position;
        vertexPositions[1] = pos + delta100;
        mvp_positions[1] = mvp_position + mvp_delta100;
        vertexPositions[2] = pos + delta110;
        mvp_positions[2] = mvp_position + mvp_delta110;
        vertexPositions[3] = pos + delta010;
        mvp_positions[3] = mvp_position + mvp_delta010;
        vertexPositions[4] = pos + delta101;
        mvp_positions[4] = mvp_position + mvp_delta101;
        vertexPositions[5] = pos + delta111;
        mvp_positions[5] = mvp_position + mvp_delta111;
        vertexPositions[6] = pos + delta011;
        mvp_positions[6] = mvp_position + mvp_delta011;
        vertexPositions[7] = pos + delta001;
        mvp_positions[7] = mvp_position + mvp_delta001;

        // Face 1 with normal (0,0,-1), indices {0,3,1,2}
        normal = vec3(0,0,-1);
        vertexPosition = vertexPositions[0];
        gl_Position = mvp_positions[0];
        EmitVertex();
        vertexPosition = vertexPositions[3];
        gl_Position = mvp_positions[3];
        EmitVertex();
        vertexPosition = vertexPositions[1];
        gl_Position = mvp_positions[1];
        EmitVertex();
        vertexPosition = vertexPositions[2];
        gl_Position = mvp_positions[2];
        EmitVertex();
        EndPrimitive();
        
        // Face 2 with normal (0,0,1), indices {4, 5, 7, 6}
        normal = vec3(0,0,1);
        vertexPosition = vertexPositions[4];
        gl_Position = mvp_positions[4];
        EmitVertex();
        vertexPosition = vertexPositions[5];
        gl_Position = mvp_positions[5];
        EmitVertex();
        vertexPosition = vertexPositions[7];
        gl_Position = mvp_positions[7];
        EmitVertex();
        vertexPosition = vertexPositions[6];
        gl_Position = mvp_positions[6];
        EmitVertex();
        EndPrimitive();

        // Face 3 with normal (1,0,0), indices {1, 2, 4, 5}
        normal = vec3(1,0,0);
        vertexPosition = vertexPositions[1];
        gl_Position = mvp_positions[1];
        EmitVertex();
        vertexPosition = vertexPositions[2];
        gl_Position = mvp_positions[2];
        EmitVertex();
        vertexPosition = vertexPositions[4];
        gl_Position = mvp_positions[4];
        EmitVertex();
        vertexPosition = vertexPositions[5];
        gl_Position = mvp_positions[5];
        EmitVertex();
        EndPrimitive();

        // Face 4 with normal (-1,0,0), indices {7, 6, 0, 3}
        normal = vec3(-1,0,0);
        vertexPosition = vertexPositions[7];
        gl_Position = mvp_positions[7];
        EmitVertex();
        vertexPosition = vertexPositions[6];
        gl_Position = mvp_positions[6];
        EmitVertex();
        vertexPosition = vertexPositions[0];
        gl_Position = mvp_positions[0];
        EmitVertex();
        vertexPosition = vertexPositions[3];
        gl_Position = mvp_positions[3];
        EmitVertex();
        EndPrimitive();

        // Face 5 with normal (0,-1,0), indices {7, 0, 4, 1}
        normal = vec3(0,-1,0);
        vertexPosition = vertexPositions[7];
        gl_Position = mvp_positions[7];
        EmitVertex();
        vertexPosition = vertexPositions[0];
        gl_Position = mvp_positions[0];
        EmitVertex();
        vertexPosition = vertexPositions[4];
        gl_Position = mvp_positions[4];
        EmitVertex();
        vertexPosition = vertexPositions[1];
        gl_Position = mvp_positions[1];
        EmitVertex();
        EndPrimitive();

        // Face 6 with normal (0,1,0), indices {3, 6, 2, 5}
        normal = vec3(0,1,0);
        vertexPosition = vertexPositions[3];
        gl_Position = mvp_positions[3];
        EmitVertex();
        vertexPosition = vertexPositions[6];
        gl_Position = mvp_positions[6];
        EmitVertex();
        vertexPosition = vertexPositions[2];
        gl_Position = mvp_positions[2];
        EmitVertex();
        vertexPosition = vertexPositions[5];
        gl_Position = mvp_positions[5];
        EmitVertex();
        EndPrimitive();
    }
}
// END noiseviewer.gsh