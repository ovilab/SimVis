uniform highp vec3 lightPosition;
varying highp vec3 normal;
varying highp float light;
varying highp vec3 color;
void main() {
  highp float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal)), 0.0, 1.0);
  gl_FragColor = vec4(color*lightValue, 1.0);
}
