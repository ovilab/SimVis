import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractCylinders {
    id: cylindersRoot
    property alias fragmentColor: shaderOutput.value
    property alias shader: _shader

    vertexShader: ShaderBuilder {
        sourceFile: "qrc:/org.compphys.SimVis/renderables/cylinders/cylinders.vsh"
        shaderType: ShaderBuilder.Vertex
    }

    geometryShader: ShaderBuilder {
        sourceFile: "qrc:/org.compphys.SimVis/renderables/cylinders/cylinders.gsh"
        shaderType: ShaderBuilder.Geometry
    }

    fragmentShader: ShaderBuilder {
        id: _shader

        // TODO add readonly or some other way to show that these are only for others to read
        shaderType: ShaderBuilder.Fragment
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
        property ShaderNode textureCoordinate: ShaderNode {
            type: "vec2"
            name: "texCoord"
            result: "texCoord";
        }
        property ShaderNode color: ShaderNode {
            type: "vec3"
            name: "color"
            result: "color";
        }

        sourceFile: "qrc:/org.compphys.SimVis/renderables/cylinders/cylinders.fsh"

        outputs: [
            ShaderOutput {
                id: shaderOutput
                type: "vec4"
                name: "cp_FragColor"
                value: StandardMaterial {
                    position: shader.position
                    color: shader.normal
                    normal: shader.normal
                }
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
    }
}

