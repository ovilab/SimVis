// BEGIN skybox.fsh
in highp vec2 textureCoordinate;
uniform sampler2D skyMapTexture;
out vec4 outColor;
// varying highp vec3 textureCoordinate;

void main()
{
    outColor = texture(skyMapTexture, textureCoordinate);
    // gl_FragColor = texture2D(skyMapTexture, textureCoordinate.xy);
}
// END skybox.fsh
