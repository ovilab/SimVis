import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractSpheres {
    property alias fragColor: _shader.fragColor
    property alias shader: _shader
    fragmentShader: ShaderBuilder {
        id: _shader
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "position"
            result: "vertexPosition";
        }
        property ShaderNode normal: ShaderNode {
            type: "vec3"
            name: "normal"
            result: "normal";
        }
        property ShaderNode texCoord: ShaderNode {
            type: "vec2"
            name: "texCoord"
            result: "texCoord";
        }
        property ShaderNode color: ShaderNode {
            type: "vec2"
            name: "texCoord"
            result: "texCoord";
        }
        property ShaderNode sphereId: ShaderNode {
            type: "float"
            name: "sphereId"
            result: "sphereId";
        }

        property ShaderNode fragColor: Diffuse {
            position: shader.position
            color: shader.normal
            normal: shader.normal
        }

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "gl_FragColor"
                value: _shader.fragColor
            }
        ]

        source: "
in vec3 color;
in vec2 texCoord;
in vec3 vertexPosition;
in float sphereId;

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
"
    }
}

