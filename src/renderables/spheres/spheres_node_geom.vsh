void main(void)
{
    vs_position = in_position;
    vs_color = in_color;
    vs_scale = in_scale;
    vs_sphereId = in_sphereId;

    gl_Position = cp_modelViewProjectionMatrix * vec4(vs_position, 1.0);
}
