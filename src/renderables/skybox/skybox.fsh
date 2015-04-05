varying highp vec3 textureCoordinate;
uniform samplerCube skyMapTexture;

void main()
{
    gl_FragColor = textureCube(skyMapTexture, textureCoordinate);
}
