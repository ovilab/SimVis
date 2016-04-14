import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import MySimulator 1.0

Entity {
    id: fsRoot
    property alias fragmentColor: _fragmentColor.value
    property alias fragmentBuilder: _fragmentBuilder
    property alias normal: _fragmentBuilder.normal
    property alias position: _fragmentBuilder.position

    property Camera camera
    Material {
        id: material

        parameters: [
            Parameter {
                name: "triangleTable"
                value: triangleTableTexture
            },
            Parameter {
                name: "threshold"
                value: 0.001
            },
            Parameter {
                name: "scale"
                value: 5.0
            }

        ]

        effect: Effect {
            techniques: Technique {
                renderPasses: RenderPass {
                    shaderProgram: ShaderProgram {
                        vertexShaderCode: loadSource("qrc:/SimVis/renderables/marchingcubes/marchingcubes.vsh")
                        geometryShaderCode: _geometryBuilder.finalShader
                        // geometryShaderCode: loadSource("qrc:/SimVis/renderables/marchingcubes/marchingcubes.gsh")
                        fragmentShaderCode: _fragmentBuilder.finalShader

                        onFragmentShaderCodeChanged: {
                            // console.log(fragmentShaderCode)
                        }
                    }

                    ShaderBuilder {
                        id: _geometryBuilder
                        property ShaderNode position: ShaderNode {
                            type: "vec3"
                            name: "position"
                            result: "position"
                        }

                        sourceFile: "qrc:/SimVis/renderables/marchingcubes/marchingcubes.gsh"
                        shaderType: ShaderBuilder.Geometry

                        outputs: [
                            ShaderOutput {
                                id: geometryEval
                                type: "float"
                                name: "shaderNodeResult"
                                value: ShaderNode {
                                    property var position: _geometryBuilder.position
                                    name: "position"
                                    type: "float"
                                    // result: "sin(2.0 *0.1* $position.x - 0.1*0.1*$position.y*$position.z) + cos(2.0 * 0.1*$position.y) + sin(2.0 * 0.1*$position.z)*cos(2.0*0.1*$position.y);"
                                    // result: "$position.y + sin($position.z) + sin($position.x);"
                                    // result: "$position.z;"
                                    result: "sin(0.1*$position.z) + sin(0.1*$position.x) + sin(0.1*$position.y);"
                                    // result: "$position.x*$position.x + $position.y*$position.y + $position.z*$position.z;"
                                }
                            }
                        ]
                    }

                    ShaderBuilder {
                        id: _fragmentBuilder

                        material: material

                        // TODO add readonly or some other way to show that these are only for others to read
                        shaderType: ShaderBuilder.Fragment

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

                        property ShaderNode lolCoord: ShaderNode {
                            type: "vec2"
                            name: "lolCoord"
                            result: "lolCoord"
                        }

                        sourceFile: "qrc:/SimVis/renderables/marchingcubes/marchingcubes.fsh"

                        outputs: [
                            ShaderOutput {
                                id: _fragmentColor
                                type: "vec4"
                                name: "fragColor"
                                value: StandardMaterial {
                                    position: _fragmentBuilder.position
                                    normal: _fragmentBuilder.normal
                                    // color: _fragmentBuilder.lolCoord
                                    lights: ShaderGroup {
                                        Nodes.Light {
                                            position: camera.position
                                        }
                                    }
                                }
                            }
                        ]
                    }
                }
            }
        }
    }

    GeometryRenderer {
        id: functionSurfaceMesh
        primitiveType: GeometryRenderer.Points

        geometry: UniformGridGeometry {
            resolution: 64
        }
    }

    Texture2D {
        id: triangleTableTexture
        minificationFilter: Texture.Nearest
        magnificationFilter: Texture.Nearest
        wrapMode {
            x: WrapMode.ClampToEdge
            y: WrapMode.ClampToEdge
        }
        generateMipMaps: false

        textureImages: MarchingCubesTableTexture {

        }
    }

    components: [
        functionSurfaceMesh,
        material
    ]
}
