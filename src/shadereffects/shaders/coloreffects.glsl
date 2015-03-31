#ifndef COLOREFFECTSGLSL
#define COLOREFFECTSGLSL

//Sepia color based on https://github.com/mattdesl/lwjgl-basics/wiki/ShaderLesson3
const vec3 SEPIA = vec3(1.2, 1.0, 0.8);

vec3 applySepia(vec3 color) {
    //convert to grayscale using NTSC conversion weights
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

    //create our sepia tone from some constant value
    vec3 sepiaColor = vec3(gray) * SEPIA;

    //again we'll use mix so that the sepia effect is at 75%
    color = mix(color, sepiaColor, 0.75);

    return color;
}
#endif
