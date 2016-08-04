varying highp vec3 modelPosition;
varying highp vec3 modelSpherePosition;
varying highp vec3 color;
varying highp float radius;

void main(void) {
    highp vec3 rayDirection = cp_cameraPosition - modelPosition;
    highp vec3 rayOrigin = modelPosition - modelSpherePosition;

    highp vec3 E = rayOrigin;
    highp vec3 D = rayDirection;

    // Sphere equation
    //      x^2 + y^2 + z^2 = r^2
    // Ray equation is
    //     P(t) = E + t*D
    // We substitute ray into sphere equation to get
    //     (Ex + Dx * t)^2 + (Ey + Dy * t)^2 + (Ez + Dz * t)^2 = r^2
    highp float r2 = radius*radius;
    highp float a = D.x*D.x + D.y*D.y + D.z*D.z;
    highp float b = 2.0*E.x*D.x + 2.0*E.y*D.y + 2.0*E.z*D.z;
    highp float c = E.x*E.x + E.y*E.y + E.z*E.z - r2;

    // discriminant of sphere equation
    highp float d = b*b - 4.0*a*c;
    if(d < 0.0) {
        discard;
    }
    highp float t = (-b + sqrt(d))/(2.0*a);
    highp vec3 sphereIntersection = rayOrigin + t * rayDirection;

    highp vec3 normal = normalize(sphereIntersection);
    highp float pi = 3.1415926535897932384626433832795;

    highp vec3 position = modelSpherePosition + sphereIntersection;

    highp vec3 light = vec3(1.0, 1.0, 1.0);
    highp vec3 newColor = color;
#ifdef CUSTOMCOLOR
    newColor = applyCustomColor(position, normal, newColor);
#endif

    #ifdef DEFAULTLIGHT
        light = defaultLight(normal, position, newColor);
    #endif

    gl_FragColor = vec4(newColor*light, 1.0);
}
