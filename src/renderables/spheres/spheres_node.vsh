cp_in vec3 in_position;
cp_in vec3 in_color;
cp_in vec2 in_texCoord;
cp_in float in_sphereId;
cp_in float in_scale;
cp_in float in_vertexId;

cp_out vec3 position;
cp_out vec3 normal;
cp_out vec2 texCoord;
cp_out vec3 color;
cp_out float sphereId;

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
