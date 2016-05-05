attribute highp vec3 vertexPosition;
attribute highp vec2 vertexTexCoord;
attribute highp float vertexId;
attribute highp vec3 pos;

uniform highp vec3 viewVector;
uniform highp vec3 upVector;
uniform highp vec3 rightVector;

uniform highp mat4 mvp;

varying highp vec3 position;
varying highp vec2 planePosition;

void main() {
    highp float a_scale = 1.0;

    position = vertexPosition + pos;

    planePosition = vertexTexCoord;

    position += 0.5*(-upVector - rightVector)*(a_scale*float(vertexId==0.0));
    position += 0.5*(-upVector + rightVector)*(a_scale*float(vertexId==1.0));
    position += 0.5*(upVector - rightVector)*(a_scale*float(vertexId==2.0));
    position += 0.5*(upVector + rightVector)*(a_scale*float(vertexId==3.0));

    gl_Position = mvp*vec4(position, 1.0);
}
