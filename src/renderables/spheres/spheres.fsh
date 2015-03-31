uniform sampler2D texture;
varying vec2 coords;
varying vec3 color;
varying float attenuationFactor;
varying vec3 vertexPosition;
varying float sphereId;

void main() {
    vec3 light = vec3(1.0, 1.0, 1.0);

#ifdef DEFAULTLIGHT
    float x = 2.0*coords.s - 1.0;
    float y = 2.0*coords.t - 1.0;
    float r2 = x*x + y*y;
    float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2

    vec3 normal = x*cp_rightVector - y*cp_upVector - z*cp_viewVector;

#ifdef SIMPLEXBUMP
    normal = simplexbump(normal, normal+vec3(sphereId));
#endif

    light = defaultLight(normal, vertexPosition, color);
#endif

    if(r2 > 0.9) {
        // 0.9 so we don't get this light circle on the back of the spheres
        discard;
    }

    gl_FragColor = vec4(color*light, 1.0);
}
