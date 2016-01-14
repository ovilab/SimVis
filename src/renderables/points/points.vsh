// BEGIN points.vsh
uniform highp float pointSize;
in highp vec4 a_position;
void main() {
    gl_PointSize = pointSize;
    gl_Position = cp_modelViewProjectionMatrix*a_position;
}
// END points.vsh