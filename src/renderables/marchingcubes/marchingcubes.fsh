#version 410

uniform sampler2D triangleTable;
uniform mat3 modelNormalMatrix;

#pragma shadernodes header

uniform vec3 eyePosition;
in vec3 position;
in vec2 lolCoord;
in vec3 normal;
out vec4 fragColor;

void main(void) {
    #pragma shadernodes body
    // vec4 value = texture(triangleTable, gl_FragCoord.xy / 2000);
    // fragColor = vec4(value.rgb, 1.0);
    // fragColor = vec4(value.g, 0.0, 0.0, 1.0);
    // fragColor = vec4(value.r, 0.2, 0.2, 1.0);
    // fragColor = vec4(value.xyz*255, 1.0);
    // fragColor = vec4(gl_FragCoord.xy / 2000, 0.0, 1.0);
    // fragColor = vec4(texture(triangleTable, vec2(0.0,0.0)).r, texture(triangleTable, vec2(0.0,0.0)).g, texture(triangleTable, vec2(0.0,0.0)).b, 1.0);
    // fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
