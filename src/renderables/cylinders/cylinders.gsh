layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;
uniform float radius;
in vec3 vs_vertex1Position[1];
in vec3 vs_vertex2Position[1];
in float vs_radius1[1];
in float vs_radius2[1];

out vec2 texCoord;
out vec3 vertexPosition;
out vec3 yDir;
out float da;

void main(void) {
    vec3 v1 = vs_vertex1Position[0];
    vec3 v2 = vs_vertex2Position[0];

    vec4 v1mvp = cp_modelViewProjectionMatrix*vec4(v1, 1.0);
    vec4 v2mvp = cp_modelViewProjectionMatrix*vec4(v2, 1.0);

    vec4 delta = v2mvp/v2mvp.w - v1mvp/v1mvp.w;
    float costheta = normalize(delta).z;
    vec4 deltaNormalized = normalize(delta);

    vec4 ortho = vec4(cross(vec3(0,0,1), deltaNormalized.xyz), 0.0);

    da = radius*costheta;

    vec3 cameraToV1 = cp_cameraPosition - v1;
    vec3 cameraToV2 = cp_cameraPosition - v2;
    float distanceToV1 = dot(cameraToV1, cameraToV1);
    float distanceToV2 = dot(cameraToV2, cameraToV2);
    if(distanceToV1 > distanceToV2) {
        // v2 closer than v1
        gl_Position = v1mvp - ortho*vs_radius1[0];
        texCoord = vec2(-1.0, 1.0);
        EmitVertex();

        gl_Position = v2mvp - ortho*vs_radius2[0];
        texCoord = vec2(1.0, 1.0);
        EmitVertex();

        gl_Position = v1mvp + ortho*vs_radius1[0];
        texCoord = vec2(-1.0, 1.0);
        EmitVertex();

        gl_Position = v2mvp + ortho*vs_radius2[0];
        texCoord = vec2(1.0, 1.0);
        EmitVertex();
        EndPrimitive();
    } else {
        // v1 closer than v2
        gl_Position = v1mvp - ortho*vs_radius1[0];
        texCoord = vec2(1.0, 1.0);
        EmitVertex();

        gl_Position = v2mvp - ortho*vs_radius2[0];
        texCoord = vec2(-1.0, 1.0);
        EmitVertex();

        gl_Position = v1mvp + ortho*vs_radius1[0];
        texCoord = vec2(1.0, 1.0);
        EmitVertex();

        gl_Position = v2mvp + ortho*vs_radius2[0];
        texCoord = vec2(-1.0, 1.0);
        EmitVertex();
        EndPrimitive();
    }
}





