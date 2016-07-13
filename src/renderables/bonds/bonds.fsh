varying highp vec3 vs_vertex1Position;
varying highp vec3 vs_vertex2Position;
varying highp float vs_sphereRadius1;
varying highp float vs_sphereRadius2;

varying highp vec3 modelPosition;
varying highp vec3 modelViewPosition;
varying highp vec3 worldPosition;
varying highp vec3 base;
varying highp vec3 end;
varying highp vec3 color;
varying highp float radiusA;
varying highp float radiusB;
varying highp float da;
varying highp mat3 cylinderBasis;
varying highp mat3 cylinderWorldBasis;

varying highp vec3 v1;
varying highp vec3 v2;

varying highp vec3 perpendicular;
varying highp vec3 biperpendicular;

highp float square(highp vec3 a) {
    return dot(a, a);
}

void main(void) {
    highp vec3 rayOrigin = vec3(0.0, 0.0, 0.0); // in modelview space
    highp vec3 rayTarget = modelViewPosition; // in modelview space
    highp vec3 rayDirection = normalize(rayOrigin - rayTarget);

    highp mat3 basis = cylinderBasis; // cylinder space basis
    highp vec3 axis = cylinderBasis[2];
    highp vec3 worldAxis = cylinderWorldBasis[2];

    // Ray target in cylinder space
    highp vec3 cylTarget = rayTarget * basis;

    // Direction from base to hitpoint in model view space
    highp vec3 diff = rayTarget - base;

    // Ray origin in cylinder basis
    highp vec3 E = diff * basis;

    // radius and length of cone
    highp float r1 = radiusA;
    highp float r2 = radiusB;
    highp float rd = r2 - r1;
    highp float l = length(base - end);

    // Ray direction in cylinder basis
    highp vec3 D = rayDirection * basis;

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
    highp float a = D.x*D.x + D.y*D.y - (rd*rd/(l*l)*D.z*D.z);
    highp float b = 2.0*E.x*D.x + 2.0*E.y*D.y - (2.0*rd*rd/(l*l)*E.z*D.z + 2.0*r1*rd/l*D.z);
    highp float c = E.x*E.x + E.y*E.y - (r1*r1 + rd*rd/(l*l)*E.z*E.z + 2.0*r1*rd/l*E.z);
    // the solutions are
    //      t = (-b +/- sqrt(b*b - 4*a*c)) / 2*a
    // the discriminant of the above equation is
    //      d = b*b - 4*a*c
    highp float d = b*b - 4.0*a*c;
    // if this is below zero, there is no solution
    // and we are outside cylinder

    if (d < 0.0) {
        //        fragcolor = vec4(0.2, 1.0, 0.2, 1.0);
        //        return;
        discard;
    }

    // solution t
    highp float distSide = (-b + sqrt(d))/(2.0*a);

    // ray-plane intersection
    // d = ((p0 - E) . n) / (D . n)
    highp float distBase = dot(base - rayTarget, axis) / dot(rayDirection, axis);
    highp float distEnd = dot(end - rayTarget, axis) / dot(rayDirection, axis);

    // produce the possible points of the solutions
    highp vec3 sidePoint = rayTarget + distSide * rayDirection;
    highp vec3 basePoint = rayTarget + distBase * rayDirection;
    highp vec3 endPoint = rayTarget + distEnd * rayDirection;

    highp float baseDistance = square(basePoint - base);
    highp float endDistance = square(endPoint - end);

    highp float screenSideDistance = square(sidePoint);
    highp float screenBaseDistance = square(basePoint);
    highp float screenEndDistance = square(endPoint);

    // Need only to solve for closest sphere
    highp vec3 spherePosition;
    highp float sphereRadius;
    if(square(rayTarget - base) < square(rayTarget - end)) {
        spherePosition = base;
        sphereRadius = vs_sphereRadius1;
    } else {
        spherePosition = end;
        sphereRadius = vs_sphereRadius2;
    }

    highp vec3 sE = rayTarget - spherePosition;
    highp vec3 sD = rayDirection;

    // Sphere equation
    //      x^2 + y^2 + z^2 = r^2
    // Ray equation is
    //     P(t) = E + t*D
    // We substitute ray into sphere equation to get
    //     (Ex + Dx * t)^2 + (Ey + Dy * t)^2 + (Ez + Dz * t)^2 = r^2
    highp float sr2 = sphereRadius*sphereRadius;
    highp float sa = sD.x*sD.x + sD.y*sD.y + sD.z*sD.z;
    highp float sb = 2.0*sE.x*sD.x + 2.0*sE.y*sD.y + 2.0*sE.z*sD.z;
    highp float sc = sE.x*sE.x + sE.y*sE.y + sE.z*sE.z - sr2;

    // discriminant of sphere equation
    highp float sd = sb*sb - 4.0*sa*sc;
    if(sd > 0.0) {
        // solution t for sphere
        highp float distSphere = (-sb + sqrt(sd))/(2.0*sa);
        highp vec3 spherePoint = rayTarget + distSphere * rayDirection;
        highp float screenSphereDistance = square(spherePoint);
        // check if sphere is closer
        if(screenSphereDistance < screenSideDistance) {
            discard;
        }
    }

    bool isSideSolution = true;

    highp vec3 normal;
    highp float dist = distSide;


    // first, test if the end cap planes are closer to the
    // screen than the solution to the cone equation
    // this is unlikely, but is necessary to ensure that we
    // don't get solutions beyond the cone epicenter ><
    if((screenBaseDistance <= screenSideDistance && screenBaseDistance <= screenEndDistance) && baseDistance < r1*r1) {
        discard;
    }
    if(screenEndDistance <= screenSideDistance && screenEndDistance <= screenBaseDistance && endDistance < r2*r2) {
        discard;
    }

    if(isSideSolution) {
        // we didn't find a cap plane that was closer,
        // so we check if the point on the side is outside the
        // length of the cylinder, i.e. outside the two caps.
        // To do this, we check the angle between the point of
        // intersection - cylinder end vector and a cap plane normal
        // (which is the cylinder axis)
        // if cos angle < 0, the point is outside of cylinder
        if(dot(sidePoint - base, axis) < 0.0) {
            discard;
        }
        if(dot(sidePoint - end, -axis) < 0.0) {
            discard;
        }
    }

    // use the found solution to produce the final version
    // of the point in cylinder space
    highp vec3 cylPoint = E + D * dist;
    highp vec3 cylPointWorld = vs_vertex1Position + cylinderWorldBasis * cylPoint;

    // use the perpendicular vector to define the texture coordinate
    highp vec3 xyPoint = vec3(cylPoint.x, cylPoint.y, 0.0);
    highp vec3 xyPointWorld = cylinderWorldBasis * xyPoint;
    highp float x = dot(xyPointWorld, perpendicular);
    highp float y = dot(xyPointWorld, biperpendicular);
    highp float radius = r1 + rd * cylPoint.z / l; // interpolates from r1 to t2 by z
    highp vec2 texCoord = vec2(0.5 + x / (2.0*radius), 0.5 + y / (2.0*radius));

    if(isSideSolution) {
        // we didn't find a solution that returned one of the caps
        // and must calculate the normal.
        // the normal is the gradient of the function defining the cone shape
        // This is found in cone space as
        //      grad = vec3(2.0*x, 2.0*y, -(rd/l*r1 + 2*rd^2/l^2*z))
        highp vec3 cylNormal = vec3(2.0*cylPoint.x,
                              2.0*cylPoint.y,
                              (2.0*(r1-r2)*(r1*(l - cylPoint.z) + r2*cylPoint.z))/(l*l));
        // we transform this to real space by using our basis matrix
        normal = cylinderWorldBasis * cylNormal;
        normal = normalize(normal);

        highp float pi = 3.1415926535897932384626433832795;
        highp float z = cylPoint.z / l;
        texCoord = vec2(z, 0.5 + atan(y, x) / (2.0 * pi));
    }

    // calculate texture coordinate
    highp vec3 position = cylPointWorld;

    highp vec3 light = vec3(1.0, 1.0, 1.0);

#ifdef DEFAULTLIGHT
    light = defaultLight(normal, position, color);
#endif

    gl_FragColor = vec4(color*light, 1.0);
}
