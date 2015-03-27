#ifndef LIGHT
#define LIGHT
// Light based on http://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
uniform highp vec4 cp_ambientColor;
uniform highp vec4 cp_diffuseColor;
uniform highp vec4 cp_specularColor;
uniform highp vec3 cp_lightPosition;
uniform highp float cp_attenuation;
uniform highp float cp_shininess;
uniform highp float cp_diffuseIntensity;
uniform highp float cp_ambientIntensity;

highp vec3 diffuse(highp vec3 normal, highp vec3 vertexPosition) {
    highp vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    highp float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    return diffuseCoefficient*cp_diffuseColor.rgb*cp_diffuseIntensity;
}

highp vec3 ambient(highp vec3 color) {
    return cp_ambientIntensity*color;
}

highp vec3 specular(highp vec3 normal, highp vec3 vertexPosition) {
    highp vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    highp vec3 reflectionVector = reflect(-surfaceToLight, normal);
    highp vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition); //also a unit vector
    highp float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    highp float specularCoefficient = pow(cosAngle, cp_shininess);

    return cp_specularColor.rgb*specularCoefficient;
}

highp vec3 diffuseAndSpecular(highp vec3 normal, highp vec3 vertexPosition) {
    // Diffuse stuff
    highp vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    highp float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));

    // Specular stuff, using surfaceToLight from diffuse
    highp float specularCoefficient = 0.0;
    if(diffuseCoefficient > 0.0) {
        highp vec3 reflectionVector = reflect(-surfaceToLight, normal);
        highp vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition); //also a unit vector
        highp float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
        specularCoefficient = pow(cosAngle, cp_shininess);
    }

    highp vec3 diffuseLight = diffuseCoefficient*cp_diffuseColor.rgb*cp_diffuseIntensity;
    highp vec3 specularLight = cp_specularColor.rgb*specularCoefficient;
    return (diffuseLight + specularLight);
}

highp float attenuation(highp vec3 vertexPosition) {
    highp float distanceToLight = distance(vertexPosition, cp_lightPosition);
    float attenuationFactor = 1.0 / (1.0 + cp_attenuation * distanceToLight * distanceToLight);
    return attenuationFactor;
}

#endif
