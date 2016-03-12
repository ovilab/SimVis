in vec3 position;
out vec4 fragcolor;
in vec2 texCoord;
uniform sampler2D triangleTable;

void main(void) {
    fragcolor = vec4(1.0, 0.0, 0.0, 1.0);
//    fragcolor = texture(triangleTable, texCoord) * 50;
}
