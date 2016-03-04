cp_in vec2 texCoord;
cp_in vec3 modelViewPosition;
cp_in vec3 worldPosition;
cp_in vec3 base;
cp_in vec3 end;
cp_in vec3 color;
//in float radius;
cp_in float radiusA;
cp_in float radiusB;
cp_in float da;
cp_in mat3 cylinderBasis;
cp_in mat3 cylinderWorldBasis;

cp_in vec3 v1;
cp_in vec3 v2;

//out vec4 fragcolor;

void main(void) {
    vec3 rayOrigin = vec3(0.0, 0.0, 0.0); // in modelview space
    vec3 rayTarget = modelViewPosition; // in modelview space
    vec3 rayDirection = normalize(rayOrigin - rayTarget);

    mat3 basis = cylinderBasis; // cylinder space basis
    vec3 axis = cylinderBasis[2];
    vec3 worldAxis = cylinderWorldBasis[2];

    // Ray target in cylinder space
    vec3 cylTarget = rayTarget * basis;

    // Direction from base to hitpoint in model view space
    vec3 diff = rayTarget - base;

    // Ray origin in cylinder basis
    vec3 E = diff * basis;

    // radius and length of cone
    float r1 = radiusA;
    float r2 = radiusB;
    float rd = r2 - r1;
    float l = length(base - end);

    // Ray direction in cylinder basis
    vec3 D = rayDirection * basis;

    // Cone equation is
    //     x^2 + y^2 = z^2
    // However, we modify this to start with r1 and end up with r2
    //      x^2 + y^2 = (r1 + rd/l * z)^2
    // where l is the length of the cylinder along the z axis
    // and rd = r2 - r1.
    // Ray equation is
    //     P(t) = E + t*D
    // We substitute ray into cylinder equation to get
    //     (Ex + Dx * t)^2 + (Ey + Dy * t)^2 = (r1 + rd / l * (Ez + Dz * t))^2
    // The result is a quadratic equation with form
    //      a*t^2 + b*t + c = 0
    // where
    //      a = Dx^2 + Dy^2 - (...)
    //      b = 2*Dx*Ex + 2*Dy*Ey - (...)
    //      c = Ex^2 + Ey^2 - (...)
    float a = D.x*D.x + D.y*D.y - (rd*rd/(l*l)*D.z*D.z);
    float b = 2.0*E.x*D.x + 2.0*E.y*D.y - (2.0*rd*rd/(l*l)*E.z*D.z + 2*r1*rd/l*D.z);
    float c = E.x*E.x + E.y*E.y - (r1*r1 + rd*rd/(l*l)*E.z*E.z + 2*r1*rd/l*E.z);
    // the solutions are
    //      t = (-b +/- sqrt(b*b - 4*a*c)) / 2*a
    // the discriminant of the above equation is
    //      d = b*b - 4*a*c
    float d = b*b - 4.0*a*c;
    // if this is below zero, there is no solution
    // and we are outside cylinder
    if (d < 0.0) {
        //        fragcolor = vec4(0.2, 1.0, 0.2, 1.0);
        //        return;
        discard;
    }
    cp_FragColor = vec4(0.0, 1.0, 1.0, 1.0);

    float dist1 = (-b + sqrt(d))/(2.0*a); // solution t
    // ray-plane intersection
    // d = ((p0 - E) . n) / (D . n)
    float dist2 = dot(base - rayTarget, axis) / dot(rayDirection, axis);
    float dist3 = dot(end - rayTarget, axis) / dot(rayDirection, axis);

    // point of intersection on cylinder surface
    vec3 newPoint1 = rayTarget + dist1 * rayDirection;
    vec3 newPoint2 = rayTarget + dist2 * rayDirection;
    vec3 newPoint3 = rayTarget + dist3 * rayDirection;

    float distanceFromBase2 = length(newPoint2 - base);
    float distanceFromBase3 = length(newPoint3 - end);

    float distance1 = length(newPoint1);
    float distance2 = length(newPoint2);
    float distance3 = length(newPoint3);

    bool doCapTests = true;

    vec3 normal;
    float dist = dist1;
    if(distance2 <= distance1 && distance2 <= distance3 && distanceFromBase2 < r1) {
        dist = dist2;
        doCapTests = false;
        normal = worldAxis;
    }
    if(distance3 <= distance1 && distance3 <= distance2 && distanceFromBase3 < r2) {
        dist = dist3;
        doCapTests = false;
        normal = -worldAxis;
    }

    vec3 newPoint = rayTarget + dist * rayDirection;

    // The new point in cylinder space
    vec3 cylPoint = E + D * dist;
    vec3 cylPointWorld = cylinderWorldBasis * cylPoint;

    if(doCapTests) {
        // the normal is the gradient of the function defining the cone shape
        // This is found in cone space as
        //      grad = vec3(2.0*x, 2.0*y, -(rd/l*r1 + 2*rd^2/l^2*z))
        vec3 cylNormal = vec3(2.0*cylPoint.x,
                              2.0*cylPoint.y,
                              (2.0*(r1-r2)*(r1*(l - cylPoint.z) + r2*cylPoint.z))/(l*l));
        // we transform this to real space by using our basis matrix
        normal = cylinderWorldBasis * cylNormal;
        normal = normalize(normal);
    }

    // to calculate caps, simply check the angle between
    // the point of intersection - cylinder end vector
    // and a cap plane normal (which is the cylinder cylinder_axis)
    // if the angle < 0, the point is outside of cylinder
    if(doCapTests) {
        // test caps
        float bottom_cap_test = dot(newPoint - base, axis);
        float top_cap_test = dot(newPoint - end, -axis);
        if (bottom_cap_test < 0.0) {
            vec3 pointB = rayTarget + rayDirection * dist2;
            vec3 diffFromBase = pointB - base;
            if (dot(diffFromBase, diffFromBase) > r1*r1)  {
                discard;
            }
            cylPointWorld = cylinderWorldBasis * (E + D * dist2);
            normal = worldAxis;
        } else if (top_cap_test < 0.0) {
            vec3 pointB = rayTarget + rayDirection * dist3;
            vec3 diffFromEnd = pointB - end;
            if (dot(diffFromEnd, diffFromEnd) > r2*r2)  {
                discard;
            }
            cylPointWorld = cylinderWorldBasis * (E + D * dist3);
            normal = -worldAxis;
        }
    }

    vec3 position = cylPointWorld;

    $setupShaderNodes();
}
