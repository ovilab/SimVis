#version 150 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec3 pos;

out vec3 vs_normal;
out vec3 vs_position;
uniform mat4 mvp;
uniform mat3 modelViewNormal;
uniform mat4 modelView;

void main()
{
    vs_normal = normalize(modelViewNormal * vertexNormal);
    vs_position = vec3(modelView * vec4(vertexPosition, 1.0));
    gl_Position = mvp*(vec4(vertexPosition + pos, 1.0));
}
