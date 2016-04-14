#version 410

in vec3 vertexPosition;
in vec3 delta;
out vec3 vs_position;
out vec3 vs_delta;
uniform mat4 mvp;
void main(void)
{
    vs_position = vertexPosition;
    vs_delta = delta;
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
