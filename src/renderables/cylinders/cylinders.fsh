in vec2 texCoord;
in vec3 vertexPosition;
in vec3 base;
in vec3 end_cyl;
//in float radius;
in float radiusA;
in float radiusB;
in float da;
in mat3 cylinderBasis;

out vec4 fragcolor;

void main(void) {
    vec3 ray_origin = vec3(0.0, 0.0, 0.0); // in modelview space
    vec3 ray_target = vertexPosition; // in modelview space
    vec3 ray_direction = normalize(ray_origin - ray_target);

    mat3 basis = cylinderBasis; // cylinder space basis
    vec3 axis = cylinderBasis[2];

    // Ray origin in cylinder space
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
    float dist = (-b + sqrt(d))/(2.0*a);

    // point of intersection on cylinder surface
    vec3 new_point = ray_target + dist * ray_direction;

    vec3 tmp_point = new_point - base;
    // TODO normal is now somewhat more diagonal
    vec3 normal = normalize(tmp_point - axis * dot(tmp_point, axis));

    // test front cap
    float cap_test = dot((new_point - base), axis);

    // to calculate caps, simply check the angle between
    // the point of intersection - cylinder end vector
    // and a cap plane normal (which is the cylinder cylinder_axis)
    // if the angle < 0, the point is outside of cylinder
    // test front cap

    // flat
    if (cap_test < 0.0)
    {
      // ray-plane intersection
      float dNV = dot(-axis, ray_direction);
      if (dNV < 0.0) {
//        fragcolor = vec4(1.0, 1.0, 0.2, 1.0);
//        return;
          discard;
      }
      float near = dot(-axis, (base)) / dNV;
      new_point = ray_direction * near + ray_origin;
      // within the cap radius?
      if (dot(new_point - base, new_point-base) > r1*r1)  {
//        fragcolor = vec4(1.0, 1.0, 1.0, 1.0);
//        return;
          discard;
      }
      normal = -axis;
    }

    // test end cap

    cap_test = dot((new_point - end_cyl), axis);

    // flat
    if (cap_test > 0.0)
    {
      // ray-plane intersection
      float dNV = dot(axis, ray_direction);
      if (dNV < 0.0) {
//          fragcolor = vec4(1.0, 0.3, 1.0, 1.0);
//          return;
          discard;
      }
      float near = dot(axis, end_cyl) / dNV;
      new_point = ray_direction * near + ray_origin;
      // within the cap radius?
      if (dot(new_point - end_cyl, new_point-base) > r2*r2) {
//          fragcolor = vec4(0.2, 0.5, 0.5, 1.0);
//          return;
          discard;
      }
      normal = axis;
    }
    fragcolor = vec4(dot(normal, ray_direction), 0.2, 0.2, 1.0);
//    fragcolor = vec4(1.0, 0.2, 0.2, 1.0);
}
