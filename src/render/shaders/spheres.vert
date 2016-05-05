#version 410

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in float vertexId;
in vec3 pos;

uniform vec3 viewVector = vec3(0.0, 0.0, 1.0);
uniform vec3 upVector = vec3(0.0, 1.0, 0.0);
uniform vec3 rightVector = vec3(1.0, 0.0, 0.0);
uniform vec3 eyePosition = vec3(0.0, 0.0, 0.0);

uniform mat4 mvp;

out vec3 position;
out vec2 planePosition;

out vec3 up;
out vec3 right;
out vec3 view;

vec3 makePerpendicular(vec3 v) {
    if(v.x == 0.0 && v.y == 0.0) {
        if(v.z == 0.0) {
            return vec3(0.0, 0.0, 0.0);
        }
        return vec3(0.0, 1.0, 0.0);
    }
    return vec3(-v.y, v.x, 0.0);
}

void main() {
    float a_scale = 1.0;

    position = vertexPosition + pos;

    view = normalize(position - eyePosition);
    right = normalize(makePerpendicular(view));
    up = cross(right, view);

    planePosition = vertexTexCoord;

    // TODO should be moved 0.5 * view closer and use ray tracing

    position += 0.5*(-up - right)*(a_scale*float(vertexId==0.0));
    position += 0.5*(-up + right)*(a_scale*float(vertexId==1.0));
    position += 0.5*(up - right)*(a_scale*float(vertexId==2.0));
    position += 0.5*(up + right)*(a_scale*float(vertexId==3.0));

    gl_Position = mvp*vec4(position, 1.0);
}
