// BEGIN default.glsl
#ifndef DEFAULTGLSL
#define DEFAULTGLSL

#if __VERSION__ >= 130

#define cp_in in
#define cp_out out
#define cp_texture texture
out vec4 cp_FragColor;

#else

#define cp_in varying
#define cp_in attribute
#define cp_texture texture2D
#define cp_FragColor gl_FragColor

#endif

uniform highp mat4 cp_modelViewMatrix;
uniform highp mat4 cp_projectionMatrix;
uniform highp mat4 cp_modelViewMatrixInverse;
uniform highp mat3 cp_normalMatrix;
uniform highp mat4 cp_projectionMatrixInverse;
uniform highp mat4 cp_modelViewProjectionMatrix;
uniform highp vec3 cp_cameraPosition;
uniform highp vec3 cp_viewVector;
uniform highp vec3 cp_upVector;
uniform highp vec3 cp_rightVector;
uniform highp float cp_time;

#endif
// END default.glsl
