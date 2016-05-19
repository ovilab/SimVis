#version 410 core
#pragma shadernodes header

in vec3 modelPosition;
in vec3 modelSpherePosition;
in vec3 color;
in vec2 planePosition;

out vec4 fragColor;

uniform mat4 modelView;
uniform mat4 inverseModelView;
uniform mat4 inverseViewMatrix;
uniform vec3 eyePosition;

uniform vec3 viewVector;

float sphereRadius = 0.5;

void main(void) {
    vec3 rayDirection = eyePosition - modelPosition;
    vec3 rayOrigin = modelPosition - modelSpherePosition;

    vec3 E = rayOrigin;
    vec3 D = rayDirection;

    // Sphere equation
    //      x^2 + y^2 + z^2 = r^2
    // Ray equation is
    //     P(t) = E + t*D
    // We substitute ray into sphere equation to get
    //     (Ex + Dx * t)^2 + (Ey + Dy * t)^2 + (Ez + Dz * t)^2 = r^2
    float r2 = sphereRadius*sphereRadius;
    float a = D.x*D.x + D.y*D.y + D.z*D.z;
    float b = 2.0*E.x*D.x + 2.0*E.y*D.y + 2.0*E.z*D.z;
    float c = E.x*E.x + E.y*E.y + E.z*E.z - r2;

    // discriminant of sphere equation
    float d = b*b - 4.0*a*c;
    if(d < 0.0) {
        discard;
    }

    float t = (-b + sqrt(d))/(2.0*a);
    vec3 sphereIntersection = rayOrigin + t * rayDirection;

    float x = sphereIntersection.x;
    float y = sphereIntersection.y;
    float z = sphereIntersection.z; // Equation for sphere, x^2 + y^2 + z^2 = R^2
    vec3 normal = normalize(sphereIntersection);
    float pi = 3.1415926535897932384626433832795;
    vec2 texCoord = vec2(0.5 + atan(-normal.z, normal.x) / (2.0 * pi), 0.5 - asin(normal.y) / pi);

    vec3 position = sphereIntersection;

#pragma shadernodes body
}
