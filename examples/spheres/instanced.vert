#version 410 core

in vec3 vertexPosition;
in vec3 pos;

out vec3 vs_color;
out vec3 vs_position;
uniform mat4 mvp;

void main()
{
//    vec3 offsetPos = vertexPosition;
//    vec3 offsetPos = vertexPosition + pos;
//    vs_position = offsetPos;
//    gl_Position = mvp*vec4(offsetPos, 1.0);
    vs_color = pos;
    vs_position = pos;
    gl_Position = mvp*(vec4(vertexPosition + pos, 1.0));
}
