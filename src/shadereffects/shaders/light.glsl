#ifndef LIGHTGLSL
#define LIGHTGLSL

// Light based on http://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
uniform highp vec4 cp_ambientColor;
uniform highp vec4 cp_diffuseColor;
uniform highp vec4 cp_specularColor;
uniform highp vec3 cp_lightPosition;
uniform highp float cp_attenuation;
uniform highp float cp_shininess;
uniform highp float cp_diffuseIntensity;
uniform highp float cp_ambientIntensity;
uniform highp float cp_specularIntensity;

highp vec3 diffuse(highp vec3 normal, highp vec3 vertexPosition, highp vec3 color) {
#ifdef DEFAULTLIGHTDIFFUSE
    highp vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    highp float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    return color*diffuseCoefficient*cp_diffuseIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

highp vec3 diffuse(highp vec3 normal, highp vec3 vertexPosition, highp vec4 color) {
    return diffuse(normal, vertexPosition, color.rgb);
}

highp vec3 ambient(highp vec3 color) {
#ifdef DEFAULTLIGHTAMBIENT
    return color*cp_ambientIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

highp vec3 ambient(highp vec4 color) {
    return ambient(color.rgb);
}

highp vec3 specular(highp vec3 normal, highp vec3 vertexPosition, highp vec3 color) {
#ifdef DEFAULTLIGHTSPECULAR
    highp vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    highp vec3 reflectionVector = reflect(-surfaceToLight, normal);
    highp vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition); //also a unit vector
    highp float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    highp float specularCoefficient = pow(cosAngle, cp_shininess);

    return color*specularCoefficient*cp_specularIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

highp vec3 specular(highp vec3 normal, highp vec3 vertexPosition, highp vec4 color) {
    return specular(normal, vertexPosition, color.rgb);
}

highp float attenuation(highp vec3 vertexPosition) {
    highp float distanceToLight = distance(vertexPosition, cp_lightPosition);
    highp float attenuationFactor = 1.0 / (1.0 + cp_attenuation * distanceToLight * distanceToLight);
    return attenuationFactor;
}

#endif
