// BEGIN spheres.fsh
uniform sampler2D texture;
varying highp vec2 coords;
varying highp vec3 color;
varying highp vec3 vertexPosition;
varying highp float sphereId;

void main() {
    highp float x = coords.s;
    highp float y = coords.t;
    highp float r2 = x*x + y*y;
    if(r2 > 1.0) {
        // 0.9 so we don't get this light circle on the back of the spheres
        discard;
    } else {
        highp float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2

        gl_FragColor = defaultFragment(normal, vertexPosition, color);
    }
}
// END spheres.fsh
