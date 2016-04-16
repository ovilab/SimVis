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
    vec4(0.995851,         0,         0,         0),
    vec4(0, -0.700208,         0,         0),
    vec4(0,         0,   4.99995,  -4.00005),
    vec4(0,         0,  -4.99995,   5.00005));

void main(void) {
    int nz = int(vs_delta[0].x); // I'm using this to send nx ny nz for now
    int nx = int(vs_delta[0].y);
    int ny = int(vs_delta[0].z);
    int index = int(vs_position[0].x);
    int i = int(vs_position[0].x);
    int j = int(vs_position[0].y);
    int k = int(vs_position[0].z);

//    int k = index / (nx*ny);
//    int i = int(mod((index/nz), ny));
//    int j = int(mod(index, nx));

    float min = -1.0;
    float max = 1.0;
    float delta = max-min;

    float dx = delta / nx;
    float dy = delta / ny;
    float dz = delta / nz;

    float x = min + i*dx;
    float y = min + j*dy;
    float z = min + k*dz;

    vec4 p1 = inverseModelViewProjection*vec4(vec3(x,y,z), 1.0);
    vec4 p2 = inverseModelViewProjection*vec4(vec3(x+dx,y,z), 1.0);
    vec4 p3 = inverseModelViewProjection*vec4(vec3(x,y+dx,z), 1.0);

    p1 = mvpInv*vec4(vec3(x,y,z), 1.0);
    p2 = mvpInv*vec4(vec3(x+dx,y,z), 1.0);
    p3 = mvpInv*vec4(vec3(x,y+dx,z), 1.0);

    normal = vec3(0,0,1);

    gl_Position = mvp*p1;
    position = p1.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = mvp*p2;
    position = p2.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = mvp*p3;
    position = p3.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();
    EndPrimitive();

    gl_Position = mvp*p1;
    position = p1.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = mvp*p3;
    position = p3.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = mvp*p2;
    position = p2.xyz;
    lolCoord = vec2(0.0, 0.0);
    EmitVertex();
    EndPrimitive();

//    int i = int(vs_position[0].x);
//    int j = int(vs_position[0].y);
//    int k = int(vs_position[0].z);


    //         d2
    //    c1 ------- c2
    //    |           |
    // d3 |           |
    //    |           |
    //    c3 ------- c4
    //


//    vec2 c1 = vec2(-1, -1);
//    vec2 c2 = vec2(1, -1);
//    vec2 c3 = vec2(-1, 1);

//    vec4  p_c1_ss = vec4(c1, -1.0, 1.0);
//    vec4  dp_c1_ss = vec4(c1, 1.0, 1.0);
//    vec4  p1 = inverseModelViewProjection*p_c1_ss;
//    vec4  p1_2 = inverseModelViewProjection*dp_c1_ss;
//    vec4  dp1 = p1_2 - p1;

//    vec4  p_c2_ss = vec4(c2, -1.0, 1.0);
//    vec4  dp_c2_ss = vec4(c2, 1.0, 1.0);
//    vec4  p2 = inverseModelViewProjection*p_c2_ss;
//    vec4  p2_2 = inverseModelViewProjection*dp_c2_ss;
//    vec4  dp2 = p2_2 - p2;

//    vec4  p_c3_ss = vec4(c3, -1.0, 1.0);
//    vec4  dp_c3_ss = vec4(c3, 1.0, 1.0);
//    vec4  p3 = inverseModelViewProjection*p_c3_ss;
//    vec4  p3_2 = inverseModelViewProjection*dp_c3_ss;
//    vec4  dp3 = p3_2 - p3;

////    dp1.w = 0;
////    dp1 = normalize(dp1);
////    dp2.w = 0;
////    dp2 = normalize(dp2);
////    dp3.w = 0;
////    dp3 = normalize(dp3);

//    float f_i = float(i) / nPhi;
//    float f_j = float(j) / nTheta;

//    vec4 d2 = p2-p1;
//    vec4 d3 = p3-p1;
//    vec4 dd1 = dp2-dp1;
//    vec4 dd2 = dp3-dp1;
//    // vec4 d5 = dp1 + dd1*f_i + dd2*f_j;
//    vec4 d5 = vec4(0.0, 0.0, -1.0, 0.0);

//    vec3 v_000 = (p1 + d2*f_i + d3*f_j + d5*k).xyz;

//    f_i = float(i+1) / nPhi;
//    f_j = float(j+1) / nTheta;
//    vec3 v_111 = (p1 + d2*f_i + d3*f_j + d5*(k+1)).xyz;
//    vec3 delta = v_111 - v_000;

//    vec3 v_001 = v_000 + vec3(0.0, 0.0, delta.z);
//    vec3 v_011 = v_000 + vec3(0.0, delta.y, delta.z);
//    vec3 v_010 = v_000 + vec3(0.0, delta.y, 0.0);
//    vec3 v_110 = v_000 + vec3(delta.x, delta.y, 0.0);
//    vec3 v_101 = v_000 + vec3(delta.x, 0.0, delta.z);
//    vec3 v_100 = v_000 + vec3(delta.x, 0.0, 0.0);

//    GridCell grid;
//    grid.p[0] = v_000;
//    grid.p[1] = v_010;
//    grid.p[2] = v_110;
//    grid.p[3] = v_100;
//    grid.p[4] = v_001;
//    grid.p[5] = v_011;
//    grid.p[6] = v_111;
//    grid.p[7] = v_101;

//    for(int i=0; i<8; i++) {
//        // grid.p[i] += eyePosition;
//        //            grid.p[i] *= scale;
//        // grid.p[i] += eyePosition;
//        grid.val[i] = eval(grid.p[i]);
//    }

//    int cubeindex = 0;

//    if (grid.val[0] < threshold) cubeindex |= 1;
//    if (grid.val[1] < threshold) cubeindex |= 2;
//    if (grid.val[2] < threshold) cubeindex |= 4;
//    if (grid.val[3] < threshold) cubeindex |= 8;
//    if (grid.val[4] < threshold) cubeindex |= 16;
//    if (grid.val[5] < threshold) cubeindex |= 32;
//    if (grid.val[6] < threshold) cubeindex |= 64;
//    if (grid.val[7] < threshold) cubeindex |= 128;

//    vec3 vertlist[12];
//    /* Find the vertices where the surface intersects the cube */
//    vertlist[0] = linterp(threshold,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
//    vertlist[1] = linterp(threshold,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
//    vertlist[2] = linterp(threshold,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
//    vertlist[3] = linterp(threshold,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
//    vertlist[4] = linterp(threshold,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
//    vertlist[5] = linterp(threshold,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
//    vertlist[6] = linterp(threshold,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
//    vertlist[7] = linterp(threshold,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
//    vertlist[8] = linterp(threshold,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
//    vertlist[9] = linterp(threshold,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
//    vertlist[10] = linterp(threshold,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
//    vertlist[11] = linterp(threshold,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

//    /* Emit triangles*/
//    int max = 15;
//    int triangleStartIndex = cubeindex;
//    for (int i=0; triTableValue(cubeindex, i) != -1; i+=3) {
//        if(i>=max) break;
//        vec3 p1 = vertlist[triTableValue(cubeindex, i)];
//        vec4 mvp_p1 = mvp*vec4(p1, 1.0);
//        vec3 n1 = calculateNormal(p1);
//        vec3 p2 = vertlist[triTableValue(cubeindex, i+1)];
//        vec4 mvp_p2 = mvp*vec4(p2, 1.0);
//        vec3 n2 = calculateNormal(p2);
//        vec3 p3 = vertlist[triTableValue(cubeindex, i+2)];
//        vec4 mvp_p3 = mvp*vec4(p3, 1.0);
//        vec3 n3 = calculateNormal(p3);

//        vec3 center = (p1 + p2 + p3) / 3.0;
//        vec3 eyeDirection = center - eyePosition;

//        vec3 U = p2 - p1;
//        vec3 V = p3 - p1;

//        vec3 N = normalize(cross(U, V));

//        float sign = dot(N, eyeDirection);

//        if(sign < 0) {
//            position = p1;
//            normal = n1;
//            gl_Position = mvp_p1;
//            lolCoord = vec2(0.0, 0.0);
//            EmitVertex();

//            position = p2;
//            normal = n2;
//            gl_Position = mvp_p2;
//            lolCoord = vec2(1.0, 0.0);
//            EmitVertex();

//            position = p3;
//            normal = n3;
//            gl_Position = mvp_p3;
//            lolCoord = vec2(1.0, 1.0);
//            EmitVertex();
//            EndPrimitive();

//        } else {
//            position = p3;
//            normal = n3;
//            gl_Position = mvp_p3;
//            lolCoord = vec2(1.0, 1.0);
//            EmitVertex();

//            position = p2;
//            normal = n2;
//            gl_Position = mvp_p2;
//            lolCoord = vec2(1.0, 0.0);
//            EmitVertex();

//            position = p1;
//            normal = n1;
//            gl_Position = mvp_p1;
//            lolCoord = vec2(0.0, 0.0);
//            EmitVertex();
//            EndPrimitive();
//        }
//    }
}

