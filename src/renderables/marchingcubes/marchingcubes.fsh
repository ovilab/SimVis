varying highp vec3 normal;
varying highp float light;
varying highp vec3 color;
varying highp vec3 vertexPosition;
varying highp float attenuationFactor;

lowp float surface4 (highp vec4 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;

    n += 1.0	* abs( snoise( coord * frequency ) );
    n += 0.5	* abs( snoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord * frequency * 4.0 ) );

    return n;
}

void main() {
#ifdef SIMPLEXTEXTURE
    lowp float n = surface4(vec4(vertexPosition, 0.1*cp_time)); // From perlin3.fsh
#endif

#ifdef SIMPLEXBUMP
    highp vec3 normal2 = simplexbump(normal, vertexPosition);
#else
    highp vec3 normal2 = normal;
#endif

#ifdef SIMPLEXTEXTURE
    gl_FragColor = vec4(ambient(color) + attenuationFactor*(diffuse(normal2, vertexPosition, color) + specular(normal2, vertexPosition, cp_specularColor))*n, 1.0);
#else
    gl_FragColor = vec4(ambient(color) + attenuationFactor*(diffuse(normal2, vertexPosition, color) + specular(normal2, vertexPosition, cp_specularColor)), 1.0);
#endif
}
