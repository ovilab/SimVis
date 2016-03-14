#version 150 core

uniform vec4 lightPosition = vec4( 0.0, 0.0, 0.0, 1.0 );
uniform vec3 lightIntensity = vec3( 1.0, 1.0, 1.0 );

uniform vec3 ka;                            // Ambient reflectivity
uniform vec3 ks = vec3( 1.0, 1.0, 1.0 );    // Specular reflectivity
uniform float shininess = 150.0;            // Specular shininess factor

in vec3 vs_position;
in vec3 vs_normal;

out vec4 fragColor;

uniform mat4 modelView;

vec3 adsModel( const in vec3 pos, const in vec3 n )
{
    // Calculate the vector from the light to the fragment
    vec3 s = normalize( vec3( lightPosition ) - pos );

    // Calculate the vector from the fragment to the eye position
    // (origin since this is in "eye" or "camera" space)
    vec3 v = normalize( -pos );

    // Reflect the light beam using the normal at this fragment
    vec3 r = reflect( -s, n );

    // Calculate the diffuse component
    float diffuse = max( dot( s, n ), 0.0 );

    // Calculate the specular component
    float specular = 0.0;
    if ( dot( s, n ) > 0.0 )
        specular = pow( max( dot( r, v ), 0.0 ), shininess );

    // Combine the ambient, diffuse and specular contributions
    return lightIntensity * ( ka + diffuse + ks * specular );
}

void main(void) {
    fragColor = vec4( adsModel( vs_position, normalize( vs_normal ) ), 1.0 );
}