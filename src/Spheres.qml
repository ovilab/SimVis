import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractSpheres {
    property alias fragColor: _shader.fragColor
    property alias shader: _shader

//    property var vertexShader: ShaderBuilder {
//        source: isGeometryShaderSupported ? "..." : "..."
//    }

//    property var geometryShader: ShaderBuilder {
//        source: "..."
//    }

    fragmentShader: ShaderBuilder {
        id: _shader
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "position"
            result: "position";
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
            type: "vec3"
            name: "color"
            result: "color";
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

//        inputs: [
//            ShaderOutput {
//                type: "vec3"
//                name: "color"
//            },
//            ShaderOutput {
//                type: "vec2"
//                name: "texCoord"
//            },
//            ShaderOutput {
//                type: "vec3"
//                name: "position"
//            },
//            ShaderOutput {
//                type: "float"
//                name: "sphereId"
//            }
//        ]

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "gl_FragColor"
                value: _shader.fragColor
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
        sourceFile: "org.compphys.SimVis/renderables/spheres/spheres.glsl"
    }
}

