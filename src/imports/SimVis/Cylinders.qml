import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractCylinders {
    id: cylindersRoot
    property alias fragColor: _shader.fragColor
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

        property ShaderNode fragColor: StandardMaterial {
            position: shader.position
            color: shader.normal
            normal: shader.normal
        }

        sourceFile: "qrc:/org.compphys.SimVis/renderables/cylinders/cylinders.fsh"

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "cp_FragColor"
                value: _shader.fragColor
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
    }
}

