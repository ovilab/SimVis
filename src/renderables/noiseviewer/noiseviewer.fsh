// BEGIN noiseviewer.fsh
in vec3 normal;
in vec3 vertexPosition;
out vec4 outColor;
void main() {
	vec3 light = vec3(1.0, 1.0, 1.0);
	vec3 color = vec3(1.0, 0.0, 0.0);
#ifdef DEFAULTLIGHT
    light = defaultLight(normal, vertexPosition, color);
#endif
	outColor = vec4(color*light, 1.0);
}
// END noiseviewer.fsh
