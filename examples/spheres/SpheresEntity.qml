import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import MySimulator 1.0

Entity {
    id: spheresRoot
    property real variable: 0.0
    property SphereData sphereData
    property Camera camera
    Material {
        id: spheresGeometryShaderTechnique
        parameters: [
            Parameter {
                name: "viewVector"
                value: camera ? camera.viewVector.normalized() : Qt.vector3d(1.0, 0.0, 0.0)
            },
            Parameter {
                name: "upVector"
                value: camera ? camera.upVector.normalized() : Qt.vector3d(0.0, 1.0, 0.0)
            },
            Parameter {
                name: "rightVector"
                value: camera ? camera.viewVector.normalized().crossProduct(camera.upVector.normalized()) : Qt.vector3d(0.0, 0.0, 1.0)
            }
        ]
//        parameters: builder.parameters
        effect: Effect {
            techniques: Technique {
                renderPasses: RenderPass {
                    bindings: [
                        ParameterMapping {
                            parameterName: "pos"
                            shaderVariableName: "pos"
                            bindingType: ParameterMapping.Attribute
                        },
                        ParameterMapping {
                            parameterName: "vertexId"
                            shaderVariableName: "vertexId"
                            bindingType: ParameterMapping.Attribute
                        }

                    ]
                    shaderProgram: ShaderProgram {
                        //                        vertexShaderCode: loadSource("qrc:/instanced.vert")
                        //                        geometryShaderCode: loadSource("qrc:/instanced.geom")
                        //                        fragmentShaderCode: loadSource("qrc:/instanced.frag")

                        vertexShaderCode: loadSource("qrc:/spheres.vert")
//                        fragmentShaderCode: loadSource("qrc:/spheres.frag")
                        fragmentShaderCode: builder.finalShader

                        onFragmentShaderCodeChanged: {
                            console.log(fragmentShaderCode)
                        }
                    }
                    ShaderBuilder {
                            id: builder

                            material: spheresGeometryShaderTechnique

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
                            property ShaderNode texCoord: ShaderNode {
                                type: "vec2"
                                name: "texCoord"
                                result: "texCoord"
                            }
                            property ShaderNode color: ShaderNode {
                                type: "vec3"
                                name: "color"
                                result: "color"
                            }
                            property ShaderNode sphereId: ShaderNode {
                                type: "float"
                                name: "sphereId"
                                result: "sphereId"
                            }

                            sourceFile: "qrc:/spheres.frag"

                            outputs: [
                                ShaderOutput {
                                    id: _fragmentColor
                                    type: "vec4"
                                    name: "fragColor"
                                    value: StandardMaterial {
                                        position: builder.position
                                        normal: builder.normal
                                        lights: ShaderGroup {
                                            Nodes.Light {}
                                            Nodes.Light {
                                                position: camera.position
                                                strength: 0.1
                                            }
                                        }
                                    }
//                                    value: Mix {
//                                        value1: "blue"
//                                        value2: "green"
//                                        mix: variable
//                                    }
                                }
                            ]

                            // TODO consider adding support for chaining shaders
                            // TODO add functionality to choose input names or shader file based on GLSL version
                        }
                }
            }
        }
    }
    GeometryRenderer {
        id: cylinderMeshInstanced
        primitiveType: GeometryRenderer.TriangleStrip
        enabled: instanceCount != 0
        instanceCount: sphereData.count

        geometry: PointGeometry {
            attributes: [
                instanceDataAttribute
            ]
        }

//        geometry: PlaneGeometry {
//            resolution: Qt.size(2, 2)
//            height: 1.0
//            width: 1.0
//            attributes: [
//                instanceDataAttribute
//            ]
//        }

        Attribute {
            id: instanceDataAttribute
            name: "pos"
            attributeType: Attribute.VertexAttribute
            dataType: Attribute.Float
            dataSize: 3
            divisor: 1
            buffer: sphereData.buffer
        }
    }
//    Transform {
//        id: transform
//        property vector3d cross: camera.viewVector.normalized().crossProduct(Qt.vector3d(0, 1, 0))
//        rotation: fromAxisAndAngle(cross.normalized(), 180/Math.PI*Math.asin(cross.length()))
//        onRotationChanged: {
//            console.log("INFO:")
//            console.log(180/Math.PI*Math.asin(cross.length()))
//            console.log(cross.normalized())
//            console.log(rotation)
//        }

////        rotation: fromAxisAndAngle(cross.normalized(), variable)
//    }
    Entity {
        components: [
            cylinderMeshInstanced,
            spheresGeometryShaderTechnique
        ]
    }
}
