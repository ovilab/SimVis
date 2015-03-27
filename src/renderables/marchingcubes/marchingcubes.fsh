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
//    lowp float n = surface4(vec4(scaledPosition.xyz,0.1)) + 1.0; // From perlin4.fsh
//    lowp float n1 = surface4(0.2*vec4(scaledPosition.xyz,time*0.0)); // From perlin4.fsh
//    lowp float n2 = surface4(0.2*vec4(scaledPosition.zxy,time*0.0)); // From perlin4.fsh
//    lowp float n3 = surface4(0.2*vec4(scaledPosition.yzx,time*0.0)); // From perlin4.fsh

    // lowp float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal+0.3*(2.0*vec3(n1, n2, n3) - 1.0))), 0.0, 1.0);
    // lowp float lightValue = 0.2 + light*clamp(dot(normalize(cp_lightPosition), normal), 0.0, 1.0);
#ifdef SIMPLEXTEXTURE
    gl_FragColor = vec4(color*lightValue*n, 1.0);
#else
    // gl_FragColor = vec4(color * (ambient() + attenuationFactor*diffuseAndSpecular(normal, vertexPosition)), 1.0);
    gl_FragColor = vec4(ambient(color) + attenuationFactor*diffuseAndSpecular(normal, vertexPosition), 1.0);
    // gl_FragColor = vec4(ambient(color) + diffuseAndSpecular(normal, vertexPosition)), 1.0);
    // gl_FragColor = vec4(color*lightValue, 1.0);
#endif
}
