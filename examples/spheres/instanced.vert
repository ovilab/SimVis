#version 410 core

in vec3 vertexPosition;
in vec3 pos;

out vec3 vs_color;
out vec3 vs_position;
uniform mat4 mvp;

void main()
{
    vec3 position = vertexPosition + pos;
    vs_color = position;
    vs_position = position;
    gl_Position = mvp*(vec4(position, 1.0));
}
