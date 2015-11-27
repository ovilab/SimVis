// BEGIN skybox.fsh
in highp vec3 textureCoordinate;
uniform samplerCube skyMapTexture;
out vec4 outColor;
void main()
{
    outColor = texture(skyMapTexture, textureCoordinate);
}
// END skybox.fsh
