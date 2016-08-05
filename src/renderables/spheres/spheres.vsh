attribute highp vec3 vertexPosition;
attribute highp float vertexId;
attribute highp float occlusion;
attribute highp vec3 col;
attribute highp float scale;

varying highp vec3 modelSpherePosition;
varying highp vec3 modelPosition;
varying highp vec3 color;
varying highp float radius;
varying highp float vs_occlusion;
uniform highp vec3 cp_upMinusRightHalf;
uniform highp vec3 cp_upPlusRightHalf;
highp vec3 makePerpendicular(vec3 v) {
    if(v.x == 0.0 && v.y == 0.0) {
        if(v.z == 0.0) {
            return vec3(0.0, 0.0, 0.0);
        }
        return vec3(0.0, 1.0, 0.0);
    }
    return vec3(-v.y, v.x, 0.0);
}

void main() {
    highp vec3 position = vertexPosition;
    color = col;
    radius = scale;
    modelSpherePosition = position;

    vec3 view = normalize(position - cp_cameraPosition);
    vec3 right = normalize(makePerpendicular(view));
    vec3 up = cross(right, view);

    position += 2.0*(-up - right)*(scale*float(vertexId==0.0));
    position += 2.0*(-up + right)*(scale*float(vertexId==1.0));
    position += 2.0*(up - right)*(scale*float(vertexId==2.0));
    position += 2.0*(up + right)*(scale*float(vertexId==3.0));

    modelPosition = position;
    vs_occlusion = occlusion;

    gl_Position = cp_modelViewProjectionMatrix*vec4(position, 1.0);
}
