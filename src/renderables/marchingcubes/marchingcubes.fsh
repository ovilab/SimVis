uniform vec3 lightPosition;
uniform float alpha;
varying vec3 normal;
varying float light;
varying vec4 color;
void main() {
  float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal)), 0.0, 1.0);
  gl_FragColor = vec4(color.xyz*lightValue, color.a*alpha);
}
