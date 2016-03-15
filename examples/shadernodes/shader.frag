#version 410 core
#pragma shadernodes header

in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform vec3 eyePosition;

void main(void) {
    $setupShaderNodes();
}
