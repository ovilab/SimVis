// BEGIN defaultfragment.glsl
#if __VERSION__ >= 130

#define cp_in in
#define cp_out out
out vec4 cp_FragColor;

#else

#define cp_in varying
#define cp_out
#define cp_FragColor gl_FragColor

#endif
// END defaultfragment.glsl
