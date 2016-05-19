#version 410 core
#pragma shadernodes header

in vec3 modelPosition;
in vec3 modelSpherePosition;
in vec3 modelViewPosition;
in vec3 modelViewSpherePosition;
in vec3 color;
in vec2 planePosition;

out vec4 fragColor;

uniform mat4 modelView;
uniform mat4 inverseModelView;
uniform mat4 inverseViewMatrix;
uniform vec3 eyePosition;

uniform vec3 viewVector;

in vec3 up;
in vec3 right;
in vec3 view;

float sphereRadius = 0.5;

void main(void) {
    vec3 rayDirection = eyePosition - modelPosition;
    vec3 rayOrigin = modelPosition - modelSpherePosition;

    vec3 sE = rayOrigin;
    vec3 sD = rayDirection;

    // Sphere equation
    //      x^2 + y^2 + z^2 = r^2
    // Ray equation is
    //     P(t) = E + t*D
    // We substitute ray into sphere equation to get
    //     (Ex + Dx * t)^2 + (Ey + Dy * t)^2 + (Ez + Dz * t)^2 = r^2
    float sr2 = sphereRadius*sphereRadius;
    float sa = sD.x*sD.x + sD.y*sD.y + sD.z*sD.z;
    float sb = 2.0*sE.x*sD.x + 2.0*sE.y*sD.y + 2.0*sE.z*sD.z;
    float sc = sE.x*sE.x + sE.y*sE.y + sE.z*sE.z - sr2;

    // discriminant of sphere equation
    float sd = sb*sb - 4.0*sa*sc;
    if(sd < 0.0) {
        discard;
    }

    float distSphere = (-sb + sqrt(sd))/(2.0*sa);
    vec3 spherePoint = rayOrigin + distSphere * rayDirection;

    float x = spherePoint.x;
    float y = spherePoint.y;
    float z = spherePoint.z; // Equation for sphere, x^2 + y^2 + z^2 = R^2
    vec3 normal = normalize(spherePoint);
    float pi = 3.1415926535897932384626433832795;
    vec2 texCoord = vec2(0.5 + atan(-normal.z, normal.x) / (2.0 * pi), 0.5 - asin(normal.y) / pi);

    vec3 position = (inverseViewMatrix * vec4(spherePoint, 1.0)).xyz;

#pragma shadernodes body
}
