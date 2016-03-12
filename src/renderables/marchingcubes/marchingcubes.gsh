float threshold = 0.8;
float delta = 0.2;
uniform sampler2D triangleTable;
out vec2 texCoord;

float eval(vec3 p) {
    return dot(p,p);
}

//Get triangle table value
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

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

in vec3 vs_position[1];
out vec3 position;

void main(void) {
    int edgeTable[256] = int[](
        0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
        0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
        0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
        0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
        0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
        0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
        0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
        0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
        0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
        0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
        0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
        0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
        0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
        0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
        0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
        0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
        0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
        0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
        0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
        0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
        0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
        0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
        0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
        0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
        0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
        0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
        0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
        0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
        0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
        0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
        0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
        0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0
    );

    vec3 v_000 = vs_position[0];                        // Corresponds to vertex 0
    vec3 v_001 = v_000 + vec3(0.0, 0.0, delta);       // Corresponds to vertex 4
    vec3 v_011 = v_000 + vec3(0.0, delta, delta);           // Corresponds to vertex 5
    vec3 v_010 = v_000 + vec3(0.0, delta, 0.0);       // Corresponds to vertex 1
    vec3 v_110 = v_000 + vec3(delta, delta, 0.0);           // Corresponds to vertex 2
    vec3 v_111 = v_000 + delta;                         // Corresponds to vertex 6
    vec3 v_101 = v_000 + vec3(delta, 0.0, delta);   // Corresponds to vertex 7
    vec3 v_100 = v_000 + vec3(delta, 0.0, 0.0);       // Corresponds to vertex 3


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
    /* Cube is entirely in/out of the surface */
    if(edgeTable[cubeindex] != 0) {
        /* Find the vertices where the surface intersects the cube */
        if ( (edgeTable[cubeindex] & 1)>0)
            vertlist[0] = linterp(threshold,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
        if ( (edgeTable[cubeindex] & 2)>0)
            vertlist[1] = linterp(threshold,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
        if ( (edgeTable[cubeindex] & 4)>0)
            vertlist[2] = linterp(threshold,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
        if ( (edgeTable[cubeindex] & 8)>0)
            vertlist[3] = linterp(threshold,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
        if ( (edgeTable[cubeindex] & 16)>0)
            vertlist[4] = linterp(threshold,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
        if ( (edgeTable[cubeindex] & 32)>0)
            vertlist[5] = linterp(threshold,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
        if ( (edgeTable[cubeindex] & 64)>0)
            vertlist[6] = linterp(threshold,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
        if ( (edgeTable[cubeindex] & 128)>0)
            vertlist[7] = linterp(threshold,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
        if ( (edgeTable[cubeindex] & 256)>0)
            vertlist[8] = linterp(threshold,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
        if ( (edgeTable[cubeindex] & 512)>0)
            vertlist[9] = linterp(threshold,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
        if ( (edgeTable[cubeindex] & 1024)>0)
            vertlist[10] = linterp(threshold,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
        if ( (edgeTable[cubeindex] & 2048)>0)
            vertlist[11] = linterp(threshold,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

        /* Emit triangles*/
        int triangleStartIndex = cubeindex;
        int count = 0;
        for (int i=0; triTableValue(cubeindex, i) != -1; i+=3) {
            vec3 p = vertlist[triTableValue(cubeindex, i)];
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            EmitVertex();

            p = vertlist[triTableValue(cubeindex, i+1)];
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            EmitVertex();

            p = vertlist[triTableValue(cubeindex, i+2)];
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            EmitVertex();
            EndPrimitive();

            count += 1;
            if(count > 6000) {
                break;
            }
        }
        if(cubeindex > 0 && cubeindex < 255) {
            vec3 p = vec3(2.0, 0.0, 0.0);
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            texCoord = vec2(1.0, 0.0);
            EmitVertex();

            p = vec3(-triTableValue(8, 1), 0.0, 0.0);
//            p = vec3(0.0, 0.0, 0.0);
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            texCoord = vec2(0.0, 0.0);
            EmitVertex();

            p = vec3(2.0, 1.0, 0.0);
            gl_Position = cp_modelViewProjectionMatrix*vec4(p, 1.0);
            texCoord = vec2(0.0, 1.0);
            EmitVertex();
            EndPrimitive();
        }
    }
}
