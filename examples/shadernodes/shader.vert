#version 410
#pragma shadernodes header

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;

uniform mat4 mvp;

void main() {
    texCoord = vertexTexCoord;
    normal = vertexNormal;

#pragma shadernodes body

    gl_Position = mvp*vec4(position, 1.0);
}
