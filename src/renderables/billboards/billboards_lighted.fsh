uniform sampler2D texture;
varying highp vec2 coords;
varying highp vec3 color;
varying highp float light;
void main() {
    vec4 shadowColor = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 picture = texture2D(texture, coords.st);
    if(picture.a < 0.9) {
        discard;
    }
    vec4 outColor = clamp(picture + light*shadowColor, 0.0, 1.0);
    gl_FragColor = outColor;
}
