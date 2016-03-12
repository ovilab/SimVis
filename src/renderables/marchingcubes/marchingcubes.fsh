in vec3 position;
out vec4 fragcolor;

void main(void) {
    fragcolor = vec4(position + vec3(0.5), 1.0);
}
