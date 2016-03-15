#version 410

in vec3 vertexPosition;
in vec3 pos;
in float vertexId;

uniform vec3 viewVector;
uniform vec3 upVector;
uniform vec3 rightVector;

uniform mat4 mvp;

out vec3 position;
out vec2 texCoord;

void main() {
    float a_scale = 1.0;

    position = vertexPosition + pos;

    texCoord = vec2(0.0, 0.0);
    vec2 up = vec2(0.0, 1.0);
    vec2 right = vec2(1.0, 0.0);
    texCoord += (-up - right)*(float(vertexId==0.0));
    texCoord += (-up + right)*(float(vertexId==1.0));
    texCoord += (up - right)*(float(vertexId==2.0));
    texCoord += (up + right)*(float(vertexId==3.0));

    position += 0.5*(-upVector - rightVector)*(a_scale*float(vertexId==0.0));
    position += 0.5*(-upVector + rightVector)*(a_scale*float(vertexId==1.0));
    position += 0.5*(upVector - rightVector)*(a_scale*float(vertexId==2.0));
    position += 0.5*(upVector + rightVector)*(a_scale*float(vertexId==3.0));

    gl_Position = mvp*vec4(position, 1.0);
}
