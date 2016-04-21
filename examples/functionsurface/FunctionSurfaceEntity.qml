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
    property real  r: 2.0
    property Camera camera
    onCameraChanged: {
        console.log("Cam pos: "+camera.position)
    }

    Material {
        id: material

        parameters: [
            Parameter {
                name: "triangleTable"
                value: triangleTableTexture
            },
            Parameter {
                name: "threshold"
                value: 0.0
            },
            Parameter {
                name: "scale"
                value: 1.0
            },
            Parameter {
                name: "r"
                value: r
            }
        ]

        effect: Effect {
            techniques: Technique {
                renderPasses: RenderPass {
                    bindings: [
                        ParameterMapping {
                            parameterName: "delta"
                            shaderVariableName: "delta"
                            bindingType: ParameterMapping.Attribute
                        }
                    ]
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
                                    // result: "$position.z + 3*sin(0.05*$position.y) + 3*sin(0.05*$position.x);"
                                    // result: "$position.x*$position.x + $position.z*$position.z;"
                                    // result: "exp(-($position.y*$position.y + $position.x*$position.x))"
                                    // result: "$position.x + $position.y + $position.z;"
                                    // result: "10*sin(0.1*($position.z+$position.x)) - $position.y"
                                    // result: "sqrt($position.y*$position.y + $position.x*$position.x + ($position.z+15.0)*($position.z+15.0))"
                                    result: "sin(0.05353522*$position.z) + sin(0.1343535*$position.x) + sin(0.075234555*$position.y);"
                                    // result: "sin($position.z)"
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
                                    ambientIntensity: 100
//                                    normal: _fragmentBuilder.normal
                                    normal: NormalMap {
                                        color: ImageTexture {
                                            source: "file:///projects/SimVis/SimVis/examples/cylinders/normal.webp"
                                            vector: Multiply {
                                                value1: _fragmentBuilder.position
                                                value2: 0.1
                                            }
                                        }
                                    }

//                                     color: _fragmentBuilder.lolCoord
                                    color: ImageTexture {
                                        source: "file:///projects/SimVis/SimVis/examples/cylinders/diffuse.webp"
                                        vector: Multiply {
                                            value1: _fragmentBuilder.position
                                            value2: 0.1
                                        }
                                    }

                                    lights: ShaderGroup {
                                        Nodes.Light {
                                            position: camera.position
                                            attenuation: 0.0
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

        geometry: UniformGridGeometry { }
//        geometry: SphereGeometry {}
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
