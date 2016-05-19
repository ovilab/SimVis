#version 410

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in float vertexId;
in vec3 pos;
in vec3 col;
in float scale;

uniform vec3 eyePosition = vec3(0.0, 0.0, 0.0);

uniform mat4 modelView;
uniform mat4 mvp;

out vec3 modelViewSpherePosition;
out vec3 position;
out vec3 modelViewPosition;
out vec3 color;
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
    position = vertexPosition + pos;
    color = col;
    modelViewSpherePosition = (modelView * vec4(position, 1.0)).xyz;

    view = normalize(position - eyePosition);
    right = normalize(makePerpendicular(view));
    up = cross(right, view);

    planePosition = vertexTexCoord;

    // TODO should be moved 0.5 * view closer and use ray tracing

    position += 0.6*(-up - right)*(scale*float(vertexId==0.0));
    position += 0.6*(-up + right)*(scale*float(vertexId==1.0));
    position += 0.6*(up - right)*(scale*float(vertexId==2.0));
    position += 0.6*(up + right)*(scale*float(vertexId==3.0));

    vec4 modelViewPositionTmp = modelView * vec4(position, 1.0);
    modelViewPosition = modelViewPositionTmp.xyz;

    gl_Position = mvp*vec4(position, 1.0);
}
