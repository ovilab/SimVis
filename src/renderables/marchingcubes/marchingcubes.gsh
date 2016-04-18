#version 410

#pragma shadernodes header

layout( points ) in;
layout( triangle_strip, max_vertices = 64 ) out;

in vec3 vs_position[1];
in vec3 vs_delta[1];
out vec3 position;
out vec3 normal;
out vec2 lolCoord;
out vec3 color;
uniform float threshold;
uniform vec3 eyePosition;
uniform float scale;
uniform sampler2D triangleTable;
uniform mat4 mvp;
uniform mat4 modelView;
uniform mat4 inverseModelViewProjection;
uniform mat4 inverseProjectionMatrix;
uniform mat4 projectionMatrix;

float eval(vec3 position) {
    float shaderNodeResult;
#pragma shadernodes body
    return shaderNodeResult;
}

vec3 calculateNormal(vec3 p) {
    // Numerical differentiation with the two point formula:
    // f' ≈ ( f(x+h) - f(x-h) ) / 2h
    float h = 0.01;
    float oneOverTwoH = 1.0 / (2.0*h);

    float fPlusX = eval(vec3(p.x + h, p.y, p.z));
    float fMinusX = eval(vec3(p.x - h, p.y, p.z));

    float fPlusY = eval(vec3(p.x, p.y+h, p.z));
    float fMinusY = eval(vec3(p.x, p.y-h, p.z));

    float fPlusZ = eval(vec3(p.x, p.y, p.z+h));
    float fMinusZ = eval(vec3(p.x, p.y, p.z-h));

    vec3 normal;
    normal.x = (fPlusX - fMinusX) * oneOverTwoH;
    normal.y = (fPlusY - fMinusY) * oneOverTwoH;
    normal.z = (fPlusZ - fMinusZ) * oneOverTwoH;
    normal = normalize(normal);
    return normal;
}

int triTableValue(int i, int j){
    return int(texelFetch(triangleTable, ivec2(i, j), 0).r * 255) - 1;
}

struct GridCell {
    vec3 p[8];
    float val[8];
};

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/
vec3 linterp(float threshold, vec3 p1, vec3 p2, float valp1, float valp2)
{
    vec3 p;

    if (abs(threshold-valp1) < 0.00001)
        return(p1);
    if (abs(threshold-valp2) < 0.00001)
        return(p2);
    if (abs(valp1-valp2) < 0.00001)
        return(p1);
    float mu = (threshold - valp1) / (valp2 - valp1);
    p = p1 + mu*(p2-p1);

    return(p);
}

vec3 translate(float r, float theta, float phi) {
    return vec3(r*sin(theta)*cos(phi), r*sin(theta)*sin(phi), r*cos(theta));
}

#define M_PI 3.1415926535897932384626433832795

mat4 mvpInv = mat4(
    vec4(0.663193,         0,         0,         0),
    vec4(       0,  0.466308,         0,         0),
    vec4(       0,         0,    -4.995,     4.005),
    vec4(       0,         0,    -4.995,     5.005));

void main(void) {
    int nx = int(vs_delta[0].x);
    int ny = int(vs_delta[0].y);
    int nz = int(vs_delta[0].z);
    int i = int(vs_position[0].x);
    int j = int(vs_position[0].y);
    int k = int(vs_position[0].z);

    float min = -1.0;

    float dx = 2.0 / nx;
    float dy = 2.0 / ny;
    float dz = 2.0 / nz;

    float x_ndc = min + i*dx; // -1 to 1
    float y_ndc = min + j*dy; // -1 to 1
    float z_ndc = min + k*dz; // -1 to 1

    // see https://www.opengl.org/wiki/Compute_eye_space_from_window_space
    float T1 = projectionMatrix[2][2];
    float T2 = projectionMatrix[2][3];
    float E1 = projectionMatrix[3][2];
    float Nz = z_ndc;
    float Pz = T2 / (T1 * Nz - T1);

    float Cw = E1 * Pz;
    float w_clip = Cw;

#define TRIANGLES
#ifdef TRIANGLES
    vec3 p1_ndc = vec3(x_ndc   ,y_ndc   , z_ndc);
    vec3 p2_ndc = vec3(x_ndc+dx,y_ndc   , z_ndc);
    vec3 p3_ndc = vec3(x_ndc   ,y_ndc+dy, z_ndc);

    vec4 p1_clip = vec4(p1_ndc * w_clip, w_clip);
    vec4 p2_clip = vec4(p2_ndc * w_clip, w_clip);
    vec4 p3_clip = vec4(p3_ndc * w_clip, w_clip);

    vec4 p1 = inverseModelViewProjection*p1_clip;
    vec4 p2 = inverseModelViewProjection*p2_clip;
    vec4 p3 = inverseModelViewProjection*p3_clip;

    p1 = mvpInv*p1_clip;
    p2 = mvpInv*p2_clip;
    p3 = mvpInv*p3_clip;

    normal = vec3(0,0,1);

    gl_Position = mvp*p1;
    position = p1.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();

    gl_Position = mvp*p2;
    position = p2.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();

    gl_Position = mvp*p3;
    position = p3.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();
    EndPrimitive();

    gl_Position = mvp*p1;
    position = p1.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();

    gl_Position = mvp*p3;
    position = p3.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();

    gl_Position = mvp*p2;
    position = p2.xyz;
    lolCoord = vec2(z_ndc, -z_ndc);
    EmitVertex();
    EndPrimitive();
#else
    vec3 v_000_ndc = vec3(x_ndc,    y_ndc,    z_ndc);
    vec3 v_111_ndc = vec3(x_ndc+dx, y_ndc+dy, z_ndc+dz);

    vec4 v_000_clip = vec4(v_000_ndc * w_ndc, w_ndc);
    vec4 v_111_clip = vec4(v_111_ndc * w_ndc, w_ndc);

    vec3 v_000 = (inverseModelViewProjection*v_000_clip).xyz;
    vec3 v_111 = (inverseModelViewProjection*v_111_clip).xyz;
//    v_000 = (mvpInv*v_000_clip).xyz;
//    v_111 = (mvpInv*v_111_clip).xyz;

    vec3 delta = v_111 - v_000;

    vec3 v_001 = v_000 + vec3(0.0, 0.0, delta.z);
    vec3 v_011 = v_000 + vec3(0.0, delta.y, delta.z);
    vec3 v_010 = v_000 + vec3(0.0, delta.y, 0.0);
    vec3 v_110 = v_000 + vec3(delta.x, delta.y, 0.0);
    vec3 v_101 = v_000 + vec3(delta.x, 0.0, delta.z);
    vec3 v_100 = v_000 + vec3(delta.x, 0.0, 0.0);

    GridCell grid;
    grid.p[0] = v_000;
    grid.p[1] = v_010;
    grid.p[2] = v_110;
    grid.p[3] = v_100;
    grid.p[4] = v_001;
    grid.p[5] = v_011;
    grid.p[6] = v_111;
    grid.p[7] = v_101;

    for(int i=0; i<8; i++) {
        // grid.p[i] += eyePosition;
        //            grid.p[i] *= scale;
        // grid.p[i] += eyePosition;
        grid.val[i] = eval(grid.p[i]);
    }

    int cubeindex = 0;

    if (grid.val[0] < threshold) cubeindex |= 1;
    if (grid.val[1] < threshold) cubeindex |= 2;
    if (grid.val[2] < threshold) cubeindex |= 4;
    if (grid.val[3] < threshold) cubeindex |= 8;
    if (grid.val[4] < threshold) cubeindex |= 16;
    if (grid.val[5] < threshold) cubeindex |= 32;
    if (grid.val[6] < threshold) cubeindex |= 64;
    if (grid.val[7] < threshold) cubeindex |= 128;

    vec3 vertlist[12];
    /* Find the vertices where the surface intersects the cube */
    vertlist[0] = linterp(threshold,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
    vertlist[1] = linterp(threshold,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
    vertlist[2] = linterp(threshold,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
    vertlist[3] = linterp(threshold,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
    vertlist[4] = linterp(threshold,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
    vertlist[5] = linterp(threshold,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
    vertlist[6] = linterp(threshold,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
    vertlist[7] = linterp(threshold,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
    vertlist[8] = linterp(threshold,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
    vertlist[9] = linterp(threshold,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
    vertlist[10] = linterp(threshold,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
    vertlist[11] = linterp(threshold,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

    /* Emit triangles*/
    int max = 15;
    int triangleStartIndex = cubeindex;
    for (int i=0; triTableValue(cubeindex, i) != -1; i+=3) {
        if(i>=max) break;
        vec3 p1 = vertlist[triTableValue(cubeindex, i)];
        vec4 mvp_p1 = mvp*vec4(p1, 1.0);
        vec3 n1 = calculateNormal(p1);
        vec3 p2 = vertlist[triTableValue(cubeindex, i+1)];
        vec4 mvp_p2 = mvp*vec4(p2, 1.0);
        vec3 n2 = calculateNormal(p2);
        vec3 p3 = vertlist[triTableValue(cubeindex, i+2)];
        vec4 mvp_p3 = mvp*vec4(p3, 1.0);
        vec3 n3 = calculateNormal(p3);

        vec3 center = (p1 + p2 + p3) / 3.0;
        vec3 eyeDirection = center - eyePosition;

        vec3 U = p2 - p1;
        vec3 V = p3 - p1;

        vec3 N = normalize(cross(U, V));

        float sign = dot(N, eyeDirection);

        if(sign < 0) {
            position = p1;
            normal = n1;
            gl_Position = mvp_p1;
            lolCoord = vec2(0.0, 0.0);
            EmitVertex();

            position = p2;
            normal = n2;
            gl_Position = mvp_p2;
            lolCoord = vec2(1.0, 0.0);
            EmitVertex();

            position = p3;
            normal = n3;
            gl_Position = mvp_p3;
            lolCoord = vec2(1.0, 1.0);
            EmitVertex();
            EndPrimitive();

        } else {
            position = p3;
            normal = n3;
            gl_Position = mvp_p3;
            lolCoord = vec2(1.0, 1.0);
            EmitVertex();

            position = p2;
            normal = n2;
            gl_Position = mvp_p2;
            lolCoord = vec2(1.0, 0.0);
            EmitVertex();

            position = p1;
            normal = n1;
            gl_Position = mvp_p1;
            lolCoord = vec2(0.0, 0.0);
            EmitVertex();
            EndPrimitive();
        }
    }
#endif
}

