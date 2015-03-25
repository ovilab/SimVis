uniform highp vec3 lightPosition;
uniform highp float alpha;
varying highp vec3 normal;
varying highp float light;
varying highp vec4 color;
void main() {
  highp float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal)), 0.0, 1.0);
  gl_FragColor = vec4(color.xyz*lightValue, color.a*alpha);
}
