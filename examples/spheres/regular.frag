#version 150 core

in vec3 vs_position;
in vec3 vs_normal;

out vec4 fragColor;

void main(void) {
    fragColor = vec4(vs_normal + vs_position, 1.0);
}
