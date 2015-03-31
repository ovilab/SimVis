#ifndef TOOLSGLSL
#define TOOLSGLSL

#ifndef HASHFLOAT
#define HASHFLOAT
float hash( float n ) {
    return fract(sin(n)*32.5454412211233);
}
#endif

#ifndef HASH2FLOAT
#define HASH2FLOAT
vec2 hash2( float n ) {
    return fract(sin(vec2(n,n+1.0))*vec2(11.1451239123,34.349430423));
}
#endif

#ifndef HASH3FLOAT
#define HASH3FLOAT
vec3 hash3( float n ) {
    return fract(sin(vec3(n,n+1.0,n+2.0))*vec3(84.54531253,42.145259123,23.349041223));
}
#endif

#ifndef HASH4FLOAT
#define HASH4FLOAT
vec3 hash4(float n )
{
    return fract(sin(vec3(n,n+1.0,n+2.0))*vec3(43758.5453123,22578.1459123,19642.3490423));
}
#endif


#ifndef SNOISE3
#define SNOISE3
vec3 snoise3(float x )
{
    float p = floor(x);
    float f = fract(x);

    f = f*f*(3.0-2.0*f);

    return -1.0 + 2.0*mix( hash4(p+0.0), hash4(p+1.0), f );
}
#endif

#ifndef PACKANDUNPACK
#define PACKANDUNPACK
vec4 pack (float depth)
{
    const vec4 bitSh = vec4(256 * 256 * 256,
                                  256 * 256,
                                  256,
                                  1.0);
    const vec4 bitMsk = vec4(0,
                                   1.0 / 256.0,
                                   1.0 / 256.0,
                                   1.0 / 256.0);
    vec4 comp = fract(depth * bitSh);
    comp -= comp.xxyz * bitMsk;
    return comp;
}


float unpack (vec4 colour)
{
    const vec4 bitShifts = vec4(1.0 / (256.0 * 256.0 * 256.0),
                                      1.0 / (256.0 * 256.0),
                                      1.0 / 256.0,
                                      1);
    return dot(colour , bitShifts);
}
#endif


#endif // TOOLSGLSL
