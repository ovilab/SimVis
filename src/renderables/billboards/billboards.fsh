uniform sampler2D texture;
varying highp vec2 coords;
varying highp vec3 color;
void main() {
    highp vec4 picture = texture2D(texture, coords.st);
    if(picture.a < 0.9) {
        discard;
    }
    gl_FragColor = picture*vec4(color, 1.0);
}
