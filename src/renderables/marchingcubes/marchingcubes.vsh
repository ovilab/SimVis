#version 410

in vec3 vertexPosition;
out vec3 vs_position;
out vec3 position;
out vec3 normal;
uniform mat4 mvp;
void main(void)
{
    gl_PointSize = 5.0;
    vs_position = vertexPosition;
    position = vertexPosition;
    normal = vec3(1.0, 0.0, 0.0);
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
