#version 410 core
#pragma shadernodes header

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec4 vertexTangent;
in vec2 vertexTexCoord;

out vec3 position;
//out vec3 normal;
out vec2 texCoord;
out mat3 tangentMatrix;

uniform mat4 mvp;
uniform mat3 modelMatrix;
uniform mat3 modelNormalMatrix;

void main() {
    texCoord = vertexTexCoord;
    position = vertexPosition;

    vec3 tangent = normalize(modelNormalMatrix * vertexTangent.xyz);
    vec3 normal = normalize(modelNormalMatrix * vertexNormal.xyz);
    vec3 binormal = normalize(cross(normal, tangent));

    tangentMatrix = mat3(tangent, binormal, normal);

#pragma shadernodes body

    gl_Position = mvp*vec4(position, 1.0);
}
