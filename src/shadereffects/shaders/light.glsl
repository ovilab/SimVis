#ifndef LIGHTGLSL
#define LIGHTGLSL

// Light based on http://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
uniform vec4 cp_ambientColor;
uniform vec4 cp_diffuseColor;
uniform vec4 cp_specularColor;
uniform vec3 cp_lightPosition;
uniform float cp_attenuation;
uniform float cp_shininess;
uniform float cp_diffuseIntensity;
uniform float cp_ambientIntensity;
uniform float cp_specularIntensity;

vec3 diffuse(vec3 normal, vec3 vertexPosition, vec3 color) {
#ifdef DEFAULTLIGHTDIFFUSE
    vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
    return color*diffuseCoefficient*cp_diffuseIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

vec3 diffuse(vec3 normal, vec3 vertexPosition, vec4 color) {
    return diffuse(normal, vertexPosition, color.rgb);
}

vec3 ambientDiffuse(vec3 normal, vec3 vertexPosition, vec3 color) {
#if defined(DEFAULTLIGHTDIFFUSE) && defined(DEFAULTLIGHTAMBIENT)
    vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition);
    float diffuseCoefficient = max(0.0, dot(normal, surfaceToCamera));
    return color*(vec3(diffuseCoefficient) + vec3(1.0))*cp_ambientIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

vec3 ambientDiffuse(vec3 normal, vec3 vertexPosition, vec4 color) {
    return ambientDiffuse(normal, vertexPosition, color.rgb);
}

vec3 ambient(vec3 color) {
#ifdef DEFAULTLIGHTAMBIENT
    return color*cp_ambientIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

vec3 ambient(vec4 color) {
    return ambient(color.rgb);
}

vec3 specular(vec3 normal, vec3 vertexPosition, vec3 color) {
#ifdef DEFAULTLIGHTSPECULAR
    vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    vec3 reflectionVector = reflect(-surfaceToLight, normal);
    vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition); //also a unit vector
    float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    float specularCoefficient = pow(cosAngle, cp_shininess);

    return color*specularCoefficient*cp_specularIntensity;
#else
    return vec3(0.0,0.0,0.0);
#endif
}

vec3 specular(vec3 normal, vec3 vertexPosition, vec4 color) {
    return specular(normal, vertexPosition, color.rgb);
}

float attenuation(vec3 vertexPosition) {
    float distanceToLight = distance(vertexPosition, cp_lightPosition);
    float attenuationFactor = 1.0 / (1.0 + cp_attenuation * distanceToLight * distanceToLight);
    return attenuationFactor;
}

vec3 defaultLight(vec3 normal, vec3 vertexPosition, vec3 color) {
    vec3 surfaceToLight = normalize(cp_lightPosition - vertexPosition);
    vec3 surfaceToCamera = normalize(cp_cameraPosition - vertexPosition);
    vec3 reflectionVector = reflect(-surfaceToLight, normal);
    float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    float specularCoefficient = pow(cosAngle, cp_shininess);

    vec3 specularColor = cp_specularColor.rgb*specularCoefficient*cp_specularIntensity;

    float diffuseCoefficient1 = max(0.0, dot(normal, surfaceToLight));
    vec3 diffuseColor = cp_diffuseColor.rgb*diffuseCoefficient1*cp_diffuseIntensity;

    float diffuseCoefficient2 = max(0.0, dot(normal, surfaceToCamera));
    vec3 ambientDiffuseColor = cp_ambientColor.rgb*(vec3(diffuseCoefficient2) + vec3(1.0))*cp_ambientIntensity;

    return color*(specularColor + ambientDiffuseColor + diffuseColor);
}

vec3 defaultLight(vec3 normal, vec3 vertexPosition, vec4 color) {
    return defaultLight(normal, vertexPosition, color.rgb);
}

#endif
