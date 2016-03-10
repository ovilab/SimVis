import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractTriangleCollection {
    id: meshRoot
    property alias fragmentColor: _fragmentShader.fragmentColor
    vertexShader: ShaderBuilder {
        sourceFile: "qrc:/org.compphys.SimVis/renderables/trianglecollection/trianglecollection.vsh"
        shaderType: ShaderBuilder.Vertex
    }

    fragmentShader: ShaderBuilder {
        id: _fragmentShader

        // TODO add readonly or some other way to show that these are only for others to read
        shaderType: ShaderBuilder.Fragment

        // inputs
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "position"
            result: "vs_position"
        }
        property ShaderNode normal: ShaderNode {
            type: "vec3"
            name: "normal"
            result: "vs_normal"
        }
        property ShaderNode color: ShaderNode {
            type: "vec3"
            name: "color"
            result: "vs_color"
        }

        // outputs
        // TODO fix this so that it can be var
        property ShaderNode fragmentColor: Diffuse {
            position: _fragmentShader.position
            color: "blue"
            normal: _fragmentShader.normal
        }

        sourceFile: "qrc:/org.compphys.SimVis/renderables/trianglecollection/trianglecollection.fsh"

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "cp_FragColor"
                value: _fragmentShader.fragmentColor
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
    }
}

