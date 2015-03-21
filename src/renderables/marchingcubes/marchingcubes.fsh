uniform vec3 lightPosition;
varying vec3 normal;
varying float light;
void main() {
  vec4 val = vec4(28.0/255.0,144.0/255.0,153.0/255.0,1.0);
  float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal)), 0.0, 1.0);
  gl_FragColor = vec4(val.xyz*lightValue, 1.0);
}
