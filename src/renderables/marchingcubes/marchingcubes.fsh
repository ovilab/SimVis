uniform highp vec3 lightPosition;
varying highp vec3 normal;
varying highp float light;
varying highp vec3 color;
varying highp vec4 scaledPosition;
uniform lowp float time;

lowp float surface3 (highp vec3 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;

    n += 1.0	* abs( cnoise( coord * frequency ) );
    n += 0.5	* abs( cnoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( cnoise( coord * frequency * 4.0 ) );

    return n;
}

lowp float surface4 (highp vec4 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;

    n += 1.0	* abs( snoise( coord * frequency ) );
    n += 0.5	* abs( snoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord * frequency * 4.0 ) );

    return n;
}

void main() {
    // lowp float n = surface3(scaledPosition.xyz*time*0.1); // From perlin3.fsh
//    lowp float n = surface4(vec4(scaledPosition.xyz,0.1)) + 1.0; // From perlin4.fsh
    lowp float n1 = surface4(0.2*vec4(scaledPosition.xyz,time*0.1)); // From perlin4.fsh
    lowp float n2 = surface4(0.2*vec4(scaledPosition.zxy,time*0.1)); // From perlin4.fsh
    lowp float n3 = surface4(0.2*vec4(scaledPosition.yzx,time*0.1)); // From perlin4.fsh

    lowp float lightValue = 0.2 + light*clamp(dot(normalize(lightPosition), normalize(normal+0.2*(2.0*vec3(n1, n2, n3) - 1.0))), 0.0, 1.0);
    gl_FragColor = vec4(color*lightValue, 1.0);
}
