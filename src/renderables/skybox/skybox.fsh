// BEGIN skybox.fsh
in highp vec3 textureCoordinate;
uniform sampler2D skyMapTexture;
out vec4 outColor;
// varying highp vec3 textureCoordinate;

void main()
{
    outColor = texture(skyMapTexture, textureCoordinate.xy);
    // gl_FragColor = texture2D(skyMapTexture, textureCoordinate.xy);
}
// END skybox.fsh
