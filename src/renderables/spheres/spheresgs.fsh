in vec3 color;
in vec2 texCoord;
in vec3 vertexPosition;
out vec4 fragcolor;

void main(void) {
    highp float x = texCoord.s;
    highp float y = texCoord.t;
    highp float r2 = x*x + y*y;
    if(r2 > 0.9) {
        // 0.9 so we don't get this light circle on the back of the spheres
        discard;
    } else {
        highp float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2

        highp vec3 light = vec3(1.0, 1.0, 1.0);
        highp vec3 normal = x*cp_rightVector + y*cp_upVector - z*cp_viewVector;

        fragcolor = defaultFragment(normal, vertexPosition, color);
    }
}
