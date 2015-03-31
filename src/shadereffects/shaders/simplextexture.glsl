#ifndef SIMPLEXTEXTUREGLSL
#define SIMPLEXTEXTUREGLSL
uniform float cp_simplexTextureScale;

float surface(vec3 coord ) {

    float frequency = 4.0;
    float n = 0.0;

#ifdef TIMEDEPENDENTSIMPLEXTEXTURE
    vec4 coord2 = vec4(coord, 0.1*cp_time);
#else
    vec3 coord2 = coord;
#endif
    coord *= cp_simplexTextureScale;

    n += 1.0	* abs( snoise( coord2 * frequency ) );
    n += 0.5	* abs( snoise( coord2 * frequency * 2.0 ) );
    n += 0.25	* abs( snoise( coord2 * frequency * 4.0 ) );

    return n;
}

#endif
