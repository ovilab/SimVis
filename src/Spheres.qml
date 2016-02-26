import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractSpheres {
    property alias fragColor: _shader.fragColor
    property alias shader: _shader
    fragmentShader: ShaderDefinition {
        id: _shader
        property Value position: Value {
            type: "vec3"
            identifier: "position"
            result: "vertexPosition";
        }
        property Value normal: Value {
            type: "vec3"
            identifier: "normal"
            result: "normal";
        }
        property Value texCoord: Value {
            type: "vec2"
            identifier: "texCoord"
            result: "texCoord";
        }
        property Value color: Value {
            type: "vec2"
            identifier: "texCoord"
            result: "texCoord";
        }

        property Value fragColor

        outputs: [
            OutputValue {
                type: "vec4"
                name: "fragColor"
                value: fragColor
            }
        ]

        source: "
    in vec3 color;
    in vec2 texCoord;
    in vec3 vertexPosition;

    void main(void) {
        highp float x = texCoord.s;
        highp float y = texCoord.t;
        highp float r2 = x*x + y*y;
        if(r2 > 0.9) {
            discard;
        } else {
            highp float z = sqrt(1.0 - r2); // Equation for sphere, x^2 + y^2 + z^2 = R^2

            highp vec3 normal = x*cp_rightVector + y*cp_upVector - z*cp_viewVector;

            $generate_shader();
        }
    }
    "
    }
}

