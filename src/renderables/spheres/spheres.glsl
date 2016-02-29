layout(location=0) in vec3 color;
layout(location=1) in vec2 texCoord;
layout(location=2) in vec3 position;
layout(location=3) in float sphereId;

//varying highp vec2 texCoord;
//varying highp vec3 color;
//varying highp vec3 vertexPosition;
//varying highp float sphereId;

void main(void) {
    highp float x = texCoord.s;
    highp float y = texCoord.t;
    highp float r2 = x*x + y*y;
    if(r2 > 1.0) {
        // 0.9 so we don't get this light circle on the back of the spheres
        discard;
    } else {
        highp float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2
        highp vec3 normal = x*cp_rightVector + y*cp_upVector - z*cp_viewVector;

        $setupShaderNodes();
    }
}
