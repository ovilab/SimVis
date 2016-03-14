#version 410 core

in vec3 vs_color[1];
in vec3 vs_position[1];
out vec3 position;
out vec3 color;
out vec2 texCoord;

uniform mat4 projectionMatrix;
uniform mat4 mvp;

layout( points ) in;
layout( triangle_strip, max_vertices = 4 ) out;

void main(void) {
    color = vs_color[0];
    position = vs_position[0];
    vec4 pos = gl_in[0].gl_Position;
    float scale = 0.5;
    gl_Position = pos + projectionMatrix*vec4(-scale, -scale, 0.0, 0.0);
    texCoord = vec2(-1.0, -1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(scale, -scale, 0.0, 0.0);
    texCoord = vec2(1.0, -1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(-scale, scale, 0.0, 0.0);
    texCoord = vec2(-1.0, 1.0);
    EmitVertex();
    gl_Position = pos + projectionMatrix*vec4(scale, scale, 0.0, 0.0);
    texCoord = vec2(1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}
