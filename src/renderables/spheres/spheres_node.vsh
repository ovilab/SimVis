// BEGIN spheres.vsh
uniform highp vec3 cp_upPlusRightHalf;
uniform highp vec3 cp_upMinusRightHalf;

void main() {
    texCoord = 2.0*in_texCoord - 1.0;
    // vertexPosition = in_position.xyz + 0.5*(cp_rightVector*coords.x + cp_upVector*coords.y);

    position = in_position.xyz;
    position -= cp_upPlusRightHalf*(in_scale*float(in_vertexId==0.0));
    position -= cp_upMinusRightHalf*(in_scale*float(in_vertexId==1.0));
    position += cp_upPlusRightHalf*(in_scale*float(in_vertexId==2.0));
    position += cp_upMinusRightHalf*(in_scale*float(in_vertexId==3.0));

    gl_Position = cp_modelViewProjectionMatrix*vec4(position, 1.0);

    sphereId = in_sphereId;

    color = in_color;
}
