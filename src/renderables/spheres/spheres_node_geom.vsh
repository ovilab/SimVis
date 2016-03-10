layout(location=0) cp_in vec3 in_position;
layout(location=1) cp_in vec3 in_color;
layout(location=2) cp_in float in_scale;
layout(location=3) cp_in float in_sphereId;

cp_out vec3 vs_position;
cp_out vec3 vs_color;
cp_out float vs_scale;
cp_out float vs_sphereId;

void main(void)
{
    vs_position = in_position;
    vs_color = in_color;
    vs_scale = in_scale;
    vs_sphereId = in_sphereId;

    gl_Position = cp_modelViewProjectionMatrix * vec4(vs_position, 1.0);
}
