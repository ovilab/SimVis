#ifndef SIMPLEXTEXTUREGLSL
#define SIMPLEXTEXTUREGLSL
uniform highp float cp_simplexTextureScale;

lowp float surface(highp vec4 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;
    coord *= cp_simplexTextureScale;

    n += 1.0	* abs( snoise( coord * frequency ) );
    n += 0.5	* abs( snoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord * frequency * 4.0 ) );

    return n;
}

lowp float surface(highp vec3 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;
    coord *= cp_simplexTextureScale;

    n += 1.0	* abs( snoise( coord * frequency ) );
    n += 0.5	* abs( snoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord * frequency * 4.0 ) );

    return n;
}

lowp float surface(highp vec2 coord ) {

    highp float frequency = 4.0;
    highp float n = 0.0;
    coord *= cp_simplexTextureScale;

    n += 1.0	* abs( snoise( coord * frequency ) );
    n += 0.5	* abs( snoise( coord * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord * frequency * 4.0 ) );

    return n;
}

#endif
