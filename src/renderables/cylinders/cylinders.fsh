in vec2 texCoord;
in vec3 vertexPosition;
in vec3 base;
in vec3 end_cyl;
in vec3 axis;
in float radius;
in float da;
in vec3 U;
in vec3 V;

out vec4 fragcolor;

void main(void) {
    vec3 ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 ray_target = vertexPosition;
    vec3 surface_point = vertexPosition;
    vec3 ray_direction = normalize(ray_origin - ray_target);

    mat3 basis = mat3(U, V, axis);

    vec3 diff = ray_target - 0.5 * (base + end_cyl);
    vec3 P = diff * basis;

    // angle (cos) between cylinder cylinder_axis and ray direction
    float dz = dot(axis, ray_direction);

    float radius2 = radius*radius;

    // calculate distance to the cylinder from ray origin
    vec3 D = vec3(dot(U, ray_direction),
                  dot(V, ray_direction),
                  dz);
    float a0 = P.x*P.x + P.y*P.y - radius2;
    float a1 = P.x*D.x + P.y*D.y;
    float a2 = D.x*D.x + D.y*D.y;
    // calculate a dicriminant of the above quadratic equation
    float d = a1*a1 - a0*a2;
    if (d < 0.0) {
//        fragcolor = vec4(0.2, 1.0, 0.2, 1.0);
//        return;
        discard;
    }
    float dist = (-a1 + sqrt(d))/a2;

    // point of intersection on cylinder surface
    vec3 new_point = ray_target + dist * ray_direction;

    vec3 tmp_point = new_point - base;
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
      if (dot(new_point - base, new_point-base) > radius2)  {
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
      if (dot(new_point - end_cyl, new_point-base) > radius2) {
//          fragcolor = vec4(0.2, 0.5, 0.5, 1.0);
//          return;
          discard;
      }
      normal = axis;
    }
    fragcolor = vec4(dot(normal, ray_direction), 0.2, 0.2, 1.0);
}
