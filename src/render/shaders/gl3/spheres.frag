#version 410 core
#pragma shadernodes header

in vec3 position;
in vec3 color;
in vec2 planePosition;

out vec4 fragColor;

uniform mat4 modelView;
uniform vec3 eyePosition;

uniform vec3 viewVector;

in vec3 up;
in vec3 right;
in vec3 view;

void main(void) {
    float x = planePosition.s;
    float y = planePosition.t;
    float r2 = x*x + y*y;
    if(r2 > 1.0) {
        discard;
    } else {
        float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2
        vec3 normal = x*right + y*up - z*view;
        float pi = 3.1415926535897932384626433832795;
        vec2 texCoord = vec2(0.5 + atan(-normal.z, normal.x) / (2.0 * pi), 0.5 - asin(normal.y) / pi);

//        fragColor = vec4( adsModel( position, normalize( normal ) ), 1.0 );
#pragma shadernodes body
    }
}
