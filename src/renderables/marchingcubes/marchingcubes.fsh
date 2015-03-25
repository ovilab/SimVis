uniform vec3 lightPosition;
varying vec3 normal;
varying float light;
varying vec3 color;
void main() {
  float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal)), 0.0, 1.0);
  gl_FragColor = vec4(color*lightValue, 1.0);
}
