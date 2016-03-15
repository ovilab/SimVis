#version 410

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

uniform mat4 mvp;

void main() {
    position = vertexPosition;
    texCoord = vertexTexCoord;
    normal = vertexNormal;

    gl_Position = mvp*vec4(position, 1.0);
}
