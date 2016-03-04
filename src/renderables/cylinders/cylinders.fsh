cp_in vec2 texCoord;
cp_in vec3 modelViewPosition;
cp_in vec3 base;
cp_in vec3 end_cyl;
cp_in vec3 color;
//in float radius;
cp_in float radiusA;
cp_in float radiusB;
cp_in float da;
cp_in mat3 cylinderBasis;
cp_in mat3 cylinderWorldBasis;

//out vec4 fragcolor;

void main(void) {
    vec3 ray_origin = vec3(0.0, 0.0, 0.0); // in modelview space
    vec3 ray_target = modelViewPosition; // in modelview space
    vec3 ray_direction = normalize(ray_origin - ray_target);

    mat3 basis = cylinderBasis; // cylinder space basis
    vec3 axis = cylinderBasis[2];
    vec3 worldAxis = cylinderWorldBasis[2];

    // Ray target in cylinder space
    vec3 cylTarget = ray_target * basis;

    // Direction from base to hitpoint in model view space
    vec3 diff = ray_target - base;

    // Ray origin in cylinder basis
    vec3 E = diff * basis;

    // radius and length of cone
    float r1 = radiusA;
    float r2 = radiusB;
    float rd = r2 - r1;
    float l = length(base - end_cyl);

    // Ray direction in cylinder basis
    vec3 D = ray_direction * basis;

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
    float dist = (-b + sqrt(d))/(2.0*a); // the solution, t


    // point of intersection on cylinder surface
    vec3 new_point = ray_target + dist * ray_direction;
    vec3 tmp_point = new_point - base;

    vec3 newCylPoint = cylTarget + dist * D;

    // The new point in cylinder space
    vec3 cylPoint = E + D * dist;
    vec3 cylPoint_world = cylinderWorldBasis * cylPoint;

    // the normal is the gradient of the function defining the cone shape
    // This is found in cone space as
    //      grad = vec3(2.0*x, 2.0*y, -(rd/l*r1 + 2*rd^2/l^2*z))
    vec3 cylNormal = vec3(2.0*cylPoint.x,
                          2.0*cylPoint.y,
                          (2.0*(r1-r2)*(r1*(l - cylPoint.z) + r2*cylPoint.z))/(l*l));
    // we transform this to real space by using our basis matrix
    vec3 normal = cylinderWorldBasis * cylNormal;
    normal = normalize(normal);

    // cylinder space
    vec3 cylBase = (base - base) * basis;
    vec3 cylEnd = (end_cyl - base)  * basis;
    vec3 cylAxis = axis * basis;

//    cp_FragColor = vec4(abs(cylEnd), 1.0);
//    return;

    // to calculate caps, simply check the angle between
    // the point of intersection - cylinder end vector
    // and a cap plane normal (which is the cylinder cylinder_axis)
    // if the angle < 0, the point is outside of cylinder

    // test bottom cap
    float bottom_cap_test = dot(cylPoint - cylBase, cylAxis);
    if (bottom_cap_test < 0.0)
    {
        // ray-plane intersection
        // d = ((p0 - E) . n) / (D . n)
        float dNV = dot(D, cylAxis);
        if (dNV > 0.0) {
//            discard;
        }
        vec3 p0 = cylBase;
        float dist2 = dot(p0-E, cylAxis) / dNV;
        vec3 cylPointB = E + D * dist2;
        cylPoint_world = cylinderWorldBasis * cylPointB;
        // within the cap radius?
        if (dot(cylPointB, cylPointB) > r1*r1)  {
            discard;
        }
        normal = -worldAxis;
    }

    // test top cap
    float top_cap_test = dot(cylPoint - cylEnd, -cylAxis);
    if (top_cap_test < 0.0)
    {
        // ray-plane intersection
        // d = ((p0 - E) . n) / (D . n)
        float dNV = dot(D, cylAxis);
        if (dNV > 0.0) {
//            discard;
        }
        vec3 p0 = cylEnd;
        float dist2 = dot(p0-E, cylAxis) / dNV;
        vec3 cylPointB = E + D * dist2;
        cylPoint_world = cylinderWorldBasis * cylPointB;
        // within the cap radius?
        if (dot(cylPointB, cylPointB) > r2*r2)  {
            discard;
        }
        normal = -worldAxis;
    }

    vec3 position = cylPoint_world;

    $setupShaderNodes();
//    fragcolor = defaultFragment(normal, cylPoint_world, color);
}
