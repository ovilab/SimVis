#version 410 core

in vec3 vertexPosition;
//in vec3 pos;

out vec3 vs_color;
out vec3 vs_position;
uniform mat4 mvp;

void main()
{
//    vs_color = pos;
//    vs_position = pos;
    vs_color = vertexPosition;
    vs_position = vertexPosition;
//    gl_Position = mvp*(vec4(vertexPosition + pos, 1.0));
    gl_Position = mvp*(vec4(vertexPosition, 1.0));
}
