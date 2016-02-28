import SimVis 1.0

ShaderNode {
    property var color: "white"

    property var ambientColor: "black"
    property var diffuseColor: color
    property var specularColor: color
    property var ambientIntensity: 0.04
    property var diffuseIntensity: 1.0
    property var specularIntensity: 0.4
    property var hardness: 10.0

    property var normal: Qt.vector3d(0.0, 0.0, 0.0)
    property var position: Qt.vector3d(0.0, 0.0, 0.0)
    property ShaderGroup lights: ShaderGroup {
        Light {}
    }

    name: "diffuse"
    type: "vec3"
    source: {
        var output = ""
        output += "$this = vec3(0.0, 0.0, 0.0);\n"
        for(var i in lights.nodes) {
            output += "$this += diffuseLight($lights[" + i + "], $(normal, vec3), $(position, vec3),\n"
            output += "                      $(ambientColor, vec3), $(diffuseColor, vec3), $(specularColor, vec3),\n"
            output += "                      $(ambientIntensity, float), $(diffuseIntensity, float), $(specularIntensity, float),\n"
            output += "                      $(hardness, float));\n"
        }
        return output
    }
    header: "
highp vec3 diffuseLight(Light light, highp vec3 normal, highp vec3 vertexPosition,
                        highp vec3 ambientColor, highp vec3 diffuseColor, highp vec3 specularColor,
                        highp float ambientIntensity, highp float diffuseIntensity, highp float specularIntensity,
                        highp float hardness) {

    highp vec3 lightVector = vec3(0.0, 0.0, 0.0);

    highp vec3 surfaceToLight = normalize(light.position - vertexPosition);
    highp float attenuationFactor = attenuation(light, vertexPosition);

    highp vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition);

   /* DIFFUSE */
    highp float diffuseCoefficient1 = max(0.0, dot(normal, surfaceToLight));
    // smooths the edges of the light (for some reason)
    diffuseCoefficient1 *= diffuseCoefficient1;
    lightVector += light.color*light.strength*diffuseColor.rgb*diffuseCoefficient1*diffuseIntensity*attenuationFactor;

   /* AMBIENTDIFFUSE */
    highp float diffuseCoefficient2 = max(0.0, dot(normal, surfaceToCamera));
    lightVector += light.color*light.strength*ambientColor.rgb*ambientIntensity*(vec3(diffuseCoefficient2)*0.9 + 0.1*vec3(1.0));

    /* SPECULAR */
    highp vec3  reflectionVector = reflect(-surfaceToLight, normal);
    highp float cosAngle = clamp(dot(surfaceToCamera, reflectionVector), 0.0, 1.0);
    highp float specularCoefficient = pow(cosAngle, hardness);
    lightVector += light.color*light.strength*specularColor.rgb*specularCoefficient*specularIntensity*attenuationFactor;

   /* RETURN GAMMA CORRECTED COMBINED */
   return gamma(lightVector, light.gamma);
}"
}
