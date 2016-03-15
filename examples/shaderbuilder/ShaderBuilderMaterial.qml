import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

Material {
    id: materialRoot

    property alias fragmentColor: _fragmentColor.value
    property alias position: shaderBuilder.position
    property alias normal: shaderBuilder.normal
    property alias texCoord: shaderBuilder.texCoord

    effect: Effect {
        techniques: Technique {
            renderPasses: RenderPass {
                shaderProgram: ShaderProgram {
                    vertexShaderCode: loadSource("qrc:/shader.vert")
                    fragmentShaderCode: shaderBuilder.finalShader
                }
            }
        }
    }
    ShaderBuilder {
        id: shaderBuilder

        shaderType: ShaderBuilder.Fragment
        material: materialRoot

        // inputs
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "position"
            result: "position"
        }
        property ShaderNode normal: ShaderNode {
            type: "vec3"
            name: "normal"
            result: "normal"
        }
        property ShaderNode texCoord: ShaderNode {
            type: "vec2"
            name: "texCoord"
            result: "texCoord"
        }

        sourceFile: "qrc:/shader.frag"

        outputs: [
            ShaderOutput {
                id: _fragmentColor
                type: "vec4"
                name: "fragColor"
                value: StandardMaterial {
                    position: shaderBuilder.position
                    normal: shaderBuilder.normal
                    lights: ShaderGroup {
                        Nodes.Light {
                            position: Qt.vector3d(5.0, 5.0, -5.0)
                        }
                    }
                }
            }
        ]
    }
}
