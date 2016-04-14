#version 410

in vec3 vertexPosition;
in vec3 normal;
in vec3 delta;
out vec3 vs_position;
//out vec3 position;
//out vec3 normal;
out vec3 vs_delta;
uniform mat4 mvp;
void main(void)
{
    vs_position = vertexPosition;
    vs_delta = delta;
//    position = vertexPosition;
//    normal = vec3(1.0, 0.0, 0.0);
    gl_Position = mvp * vec4(vertexPosition, 1.0);
}
