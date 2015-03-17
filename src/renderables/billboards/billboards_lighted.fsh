uniform sampler2D texture;
uniform highp vec4 shadowColor;
varying highp vec2 coords;
varying highp vec3 color;
varying highp float light;
void main() {
    vec4 picture = texture2D(texture, coords.st);
    if(picture.a < 0.9) {
        discard;
    }
    vec4 outColor = clamp(light*picture + (1.0 - light)*shadowColor, 0.0, 1.0);
    gl_FragColor = outColor;
}
