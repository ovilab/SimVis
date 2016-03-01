cp_in vec3 vs_position[1];
cp_in vec3 vs_color[1];
cp_in float vs_scale[1];
cp_in float vs_sphereId[1];

cp_out vec3 position;
cp_out vec3 normal;
cp_out vec2 texCoord;
cp_out vec3 color;
cp_out float sphereId;

#ifdef ADDPERIODICCOPIES
layout(invocations=27) in;
#endif
layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

void main(void) {
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
