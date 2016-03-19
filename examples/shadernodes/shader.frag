#version 410 core
#pragma shadernodes header

in vec3 position;
in vec2 texCoord;
//in vec3 normal;

in mat3 tangentMatrix;

out vec4 fragColor;

uniform vec3 eyePosition;
uniform mat3 modelNormalMatrix;

void main(void) {
    vec3 normal = tangentMatrix[2];
#pragma shadernodes body
}
