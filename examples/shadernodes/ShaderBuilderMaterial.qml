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
    property alias vertexPosition: _position.value

    property alias vertex: vertexShaderBuilder
    property alias fragment: shaderBuilder

    effect: Effect {
        techniques: Technique {
            renderPasses: RenderPass {
                shaderProgram: ShaderProgram {
                    vertexShaderCode: vertexShaderBuilder.finalShader
                    fragmentShaderCode: shaderBuilder.finalShader
                    onFragmentShaderCodeChanged: {
//                        console.log(fragmentShaderCode)
                    }
                }
            }
        }
    }
    ShaderBuilder {
        id: vertexShaderBuilder

        shaderType: ShaderBuilder.Fragment
        material: materialRoot

        // inputs
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "vertexPosition"
            result: "vertexPosition"
        }
        property ShaderNode normal: ShaderNode {
            type: "vec3"
            name: "vertexNormal"
            result: "vertexNormal"
        }
        property ShaderNode textureCoordinate: ShaderNode {
            type: "vec2"
            name: "vertexTexCoord"
            result: "vertexTexCoord"
        }

        sourceFile: "qrc:/shader.vert"

        outputs: [
            ShaderOutput {
                id: _position
                type: "vec3"
                name: "position"
                value: vertexShaderBuilder.position
            }
        ]
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
        property ShaderNode textureCoordinate: ShaderNode {
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
