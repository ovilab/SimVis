import SimVis 1.0

ShaderNode {
    property var ambientColor: Qt.vector3d(1.0, 1.0, 1.0)
    property var diffuseColor: Qt.vector3d(1.0, 1.0, 1.0)
    property var specularColor: Qt.vector3d(1.0, 1.0, 1.0)
    property var position: Qt.vector3d(0.5, 0.5, 0.5)
    property var attenuation: 0.01
    property var shininess: 2.1
    property var gamma: 1.0
    property var ambientIntensity: 0.05
    property var diffuseIntensity: 1.1
    property var specularIntensity: 0.1

    name: "light"
    type: "Light"
    source: "
Light $light;\n
$light.ambientColor = $ambientColor;
$light.specularColor = $specularColor;
$light.diffuseColor = $diffuseColor;
$light.ambientIntensity = $ambientIntensity;
$light.diffuseIntensity = $diffuseIntensity;
$light.specularIntensity = $specularIntensity;
$light.shininess = $shininess;
$light.attenuation = $attenuation;
$light.position = $position;
$light.gamma = $gamma;
"
    result: "$light"

    header: "
#ifndef LIGHTNODE_GLSL
#define LIGHTNODE_GLSL

struct Light {
    highp vec3 ambientColor;
    highp vec3 diffuseColor;
    highp vec3 specularColor;
    highp vec3 position;
    highp float attenuation;
    highp float shininess;
    highp float gamma;
    highp float diffuseIntensity;
    highp float ambientIntensity;
    highp float specularIntensity;
};

highp float attenuation(Light light, highp vec3 vertexPosition) {
    highp float distanceToLight = distance(vertexPosition, light.position);
    highp float attenuationFactor = 1.0 / (1.0 + light.attenuation * distanceToLight * distanceToLight);
    return attenuationFactor;
}

highp vec3 gamma(highp vec3 color, highp float gamma) {
    return pow(color, vec3(1.0/gamma));
}

highp vec3 applyLight(Light light, highp vec3 normal, highp vec3 vertexPosition, highp vec3 color) {
    highp vec3 lightVector = vec3(0.0, 0.0, 0.0);

    highp vec3 surfaceToLight = normalize(light.position - vertexPosition);
    highp float attenuationFactor = 1.0; //attenuation(light, vertexPosition);

    highp vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition);

   /* DIFFUSE */
    highp float diffuseCoefficient1 = max(0.0, dot(normal, surfaceToLight));
//     smooths the edges of the light (for some reason)
    diffuseCoefficient1 *= diffuseCoefficient1;
    lightVector += color*light.diffuseColor.rgb*diffuseCoefficient1*light.diffuseIntensity*attenuationFactor;

   /* AMBIENTDIFFUSE */
    highp float diffuseCoefficient2 = max(0.0, dot(normal, surfaceToCamera));
    lightVector += color*light.ambientColor.rgb*light.ambientIntensity*(vec3(diffuseCoefficient2)*0.9 + 0.1*vec3(1.0));

    /* SPECULAR */
    highp vec3  reflectionVector = reflect(-surfaceToLight, normal);
    highp float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    highp float specularCoefficient = pow(cosAngle, light.shininess);
    lightVector += light.specularColor.rgb*specularCoefficient*light.specularIntensity*attenuationFactor;

   /* RETURN GAMMA CORRECTED COMBINED */
   return gamma(lightVector, light.gamma);
}
#endif // LIGHTNODE_GLSL
"
}
