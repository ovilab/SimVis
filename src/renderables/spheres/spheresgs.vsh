layout(location=0) in vec3 in_position;
layout(location=1) in vec3 in_color;
layout(location=2) in float in_scale;
layout(location=3) in float in_sphereId;

layout(location=0) out vec3 color;
layout(location=1) out float scale;
layout(location=2) out vec3 position;
layout(location=3) out float sphereId;

void main(void)
{
    position = in_position;
    color = in_color;
    scale = in_scale;
    sphereId = in_sphereId;

    gl_Position = cp_modelViewProjectionMatrix * vec4(position, 1.0);
}
