#ifdef ADDPERIODICCOPIES
layout(invocations=27) in;
#endif
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

layout(location=0) in vec3 in_color[1];
layout(location=1) in float in_scale[1];
layout(location=2) in vec3 in_position[1];
layout(location=3) in float in_sphereId[1];

layout(location=0) out vec3 color;
layout(location=1) out vec2 texCoord;
layout(location=2) out vec3 position;
layout(location=3) out float sphereId;

void main(void) {
    vec4 displacement = vec4(0.0,0.0,0.0,0.0);
    sphereId = in_sphereId[0];
#ifdef ADDPERIODICCOPIES
    int x = gl_InvocationID % 3 - 1;
    int y = (gl_InvocationID/3)%3-1;
    int z = (gl_InvocationID/9)-1;
    displacement = vec4(systemSize.x*x, systemSize.y*y, systemSize.z*z, 0.0);
    vec4 vertex = cp_modelViewMatrix * (vec4(in_position[0], 1.0) + displacement);
    vec4 pos = gl_in[0].gl_Position + cp_modelViewProjectionMatrix * displacement;
#else
    vec4 pos = gl_in[0].gl_Position;
#endif

    bool addPoint = true;
#ifdef SLICE
    addPoint = slice_vectorIsInside(in_position[0] + displacement.xyz);
#endif
    if(addPoint) {
        position = in_position[0] + displacement.xyz;
        float scale = in_scale[0];
        color = in_color[0];

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
