highp vec3 standardMaterialLight(Light light, highp vec3 normal, highp vec3 vertexPosition, highp vec3 cameraPosition,
                        highp vec3 ambientColor, highp vec3 diffuseColor, highp vec3 specularColor,
                        highp float ambientIntensity, highp float diffuseIntensity, highp float specularIntensity,
                        highp float hardness) {
    highp vec3 lightVector = vec3(0.0, 0.0, 0.0);

    highp vec3 surfaceToLight = normalize(light.position - vertexPosition);
    highp float attenuationFactor = attenuation(light, vertexPosition);

    highp vec3 surfaceToCamera = normalize(cameraPosition - vertexPosition);

    /* DIFFUSE */
    highp float diffuseCoefficient1 = max(0.0, dot(normal, surfaceToLight));
    lightVector += light.color*light.strength*diffuseColor.rgb*diffuseCoefficient1*diffuseIntensity*attenuationFactor;

    /* AMBIENTDIFFUSE */
    highp float diffuseCoefficient2 = max(0.0, dot(normal, surfaceToCamera));
    lightVector += light.color*light.strength*ambientColor.rgb*ambientIntensity*(vec3(diffuseCoefficient2)*0.9 + 0.1*vec3(1.0));

    /* SPECULAR */
    highp vec3  reflectionVector = reflect(-surfaceToLight, normal);
    highp float cosAngle = dot(reflectionVector, surfaceToCamera);
    highp float normFactor = (hardness + 2.0) / 2.0;
    // normFactor *= (cosAngle < 3.1415) ? 1.0 : 0.0;
    highp float specularCoefficient = pow(max(cosAngle, 0.0), hardness);
    lightVector += normFactor*light.color*light.strength*specularColor.rgb*specularCoefficient*specularIntensity*attenuationFactor;

   /* RETURN GAMMA CORRECTED COMBINED */
   return gamma(lightVector, light.gamma);
}
