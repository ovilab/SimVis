#version 410

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in float vertexId;
in vec3 pos;

uniform vec3 viewVector = vec3(0.0, 0.0, 1.0);
uniform vec3 upVector = vec3(0.0, 1.0, 0.0);
uniform vec3 rightVector = vec3(1.0, 0.0, 0.0);

uniform mat4 mvp;

out vec3 position;
out vec2 texCoord;

void main() {
    float a_scale = 1.0;

    position = vertexPosition + pos;

    texCoord = vertexTexCoord;

    position += 0.5*(-upVector - rightVector)*(a_scale*float(vertexId==0.0));
    position += 0.5*(-upVector + rightVector)*(a_scale*float(vertexId==1.0));
    position += 0.5*(upVector - rightVector)*(a_scale*float(vertexId==2.0));
    position += 0.5*(upVector + rightVector)*(a_scale*float(vertexId==3.0));

    gl_Position = mvp*vec4(position, 1.0);
}
