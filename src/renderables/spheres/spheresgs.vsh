layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in float in_scale;
layout(location=3) in float in_sphereId;

out vec3 vs_color;
out float vs_scale;
out vec3 vs_position;
out float vs_sphereId;

void main(void)
{
    vs_position = in_position;
    vs_color = in_color;
    vs_scale = in_scale;
    vs_sphereId = in_sphereId;

    gl_Position = cp_modelViewProjectionMatrix * vec4(in_position, 1.0);
}
