#version 410 core
#pragma shadernodes header

uniform vec4 lightPosition = vec4( 1.0, 1.0, 1.0, 1.0 );
uniform vec3 lightIntensity = vec3( 1.0, 1.0, 1.0 );

uniform vec3 ka = vec3(0.1, 0.1, 0.1);                            // Ambient reflectivity
uniform vec3 ks = vec3( 1.0, 1.0, 1.0 );    // Specular reflectivity
uniform float shininess = 150.0;            // Specular shininess factor

in vec3 position;
in vec3 color;
in vec2 planePosition;

out vec4 fragColor;

uniform mat4 modelView;
uniform vec3 eyePosition;

uniform vec3 viewVector;
uniform vec3 rightVector;
uniform vec3 upVector;

in vec3 up;
in vec3 right;
in vec3 view;

vec3 adsModel( const in vec3 pos, const in vec3 n )
{
    // Calculate the vector from the light to the fragment
//    vec3 s = normalize( vec3( lightPosition ) - pos );
    vec3 lightpos = eyePosition;
    vec3 s = normalize(vec3(lightpos) - pos);

    // Calculate the vector from the fragment to the eye position
    // (origin since this is in "eye" or "camera" space)
    vec3 v = normalize( eyePosition-pos );

    // Calculate the diffuse component
    float diffuse = max( 0.0, dot( n, s ) );

    // Combine the ambient, diffuse and specular contributions
    return lightIntensity * ( ka + diffuse );
}

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
