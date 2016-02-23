layout( points ) in;
layout( triangle_strip, max_vertices = 12 ) out;
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

    vec3 delta = v2 - v1;
    vec3 deltaNormalized = normalize(delta);

    vec3 ortho = cross(cp_viewVector, deltaNormalized);
    ortho = normalize(ortho);

    vec3 ortho2 = cross(ortho, deltaNormalized);
    ortho2 = normalize(ortho2);

    float sign = 1.0;
    vec4 v1mvp = cp_modelViewProjectionMatrix*vec4(v1, 1.0);
    vec4 v2mvp = cp_modelViewProjectionMatrix*vec4(v2, 1.0);
    if(v1mvp.z/v1mvp.w < v2mvp.z/v2mvp.w) {
        sign = -1.0;
    }

    // top cap

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v1 - ortho*vs_radius1[0] + sign*ortho2*vs_radius1[0], 1.0));
    texCoord = vec2(-1.0, 1.0);
    EmitVertex();

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v1 + ortho*vs_radius1[0] + sign*ortho2*vs_radius1[0], 1.0));
    texCoord = vec2(1.0, 1.0);
    EmitVertex();

    // main body
    gl_Position = cp_modelViewProjectionMatrix*(vec4(v1 - ortho*vs_radius1[0], 1.0));
    texCoord = vec2(-1.0, 0.0);
    EmitVertex();

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v1 + ortho*vs_radius1[0], 1.0));
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v2 - ortho*vs_radius2[0], 1.0));
    texCoord = vec2(-1.0, 0.0);
    EmitVertex();

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v2 + ortho*vs_radius2[0], 1.0));
    texCoord = vec2(1.0, 0.0);
    EmitVertex();

    // bottom cap
    gl_Position = cp_modelViewProjectionMatrix*(vec4(v2 - ortho*vs_radius1[0] - sign*ortho2*vs_radius1[0], 1.0));
    texCoord = vec2(-1.0, 1.0);
    EmitVertex();

    gl_Position = cp_modelViewProjectionMatrix*(vec4(v2 + ortho*vs_radius1[0] - sign*ortho2*vs_radius1[0], 1.0));
    texCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();

}





