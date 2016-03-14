#version 410 core

uniform vec4 lightPosition = vec4( 0.0, 0.0, 0.0, 1.0 );
uniform vec3 lightIntensity = vec3( 1.0, 1.0, 1.0 );

uniform vec3 ka;                            // Ambient reflectivity
uniform vec3 ks = vec3( 1.0, 1.0, 1.0 );    // Specular reflectivity
uniform float shininess = 150.0;            // Specular shininess factor

in vec3 position;
in vec3 color;
in vec2 texCoord;

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
    float x = texCoord.s;
    float y = texCoord.t;
    float r2 = x*x + y*y;
    if(r2 > 1.0) {
        // 0.9 so we don't get this light circle on the back of the spheres
        discard;
    } else {
        float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2
        // TODO fix this
        vec3 normal = x*(modelView[0].xyz) + y*(modelView[1].xyz) - z*(modelView[2].xyz);
//        vec3 normal = vec3(0.0, 0.0, 1.0);

        fragColor = vec4( adsModel( position, normalize( normal ) ), 1.0 );
//        fragColor = vec4(normalize(normal), 1.0);
    }
}
