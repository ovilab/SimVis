import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2

Entity {
    id: cylindersRoot
    property var variable: 0.0
    property alias fragmentColor: _fragmentColor.value
    property alias fragmentBuilder: _fragmentBuilder
    property alias normal: _fragmentBuilder.normal
    property alias position: _fragmentBuilder.position

    property CylinderData cylinderData
    property SphereData sphereData
    property Camera camera
    Material {
        id: material
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
        effect: Effect {
            techniques: Technique {
                renderPasses: RenderPass {
                    bindings: [
                                                ParameterMapping {
                                                    parameterName: "vertex1Position"
                                                    shaderVariableName: "pos"
                                                    bindingType: ParameterMapping.Attribute
                                                }
                        //                        ParameterMapping {
                        //                            parameterName: "vertex2Position"
                        //                            shaderVariableName: "vertex2Position"
                        //                            bindingType: ParameterMapping.Attribute
                        //                        },
                        //                        ParameterMapping {
                        //                            parameterName: "radius1"
                        //                            shaderVariableName: "vertex1Position"
                        //                            bindingType: ParameterMapping.Attribute
                        //                        },
                        //                        ParameterMapping {
                        //                            parameterName: "radius2"
                        //                            shaderVariableName: "vertex1Position"
                        //                            bindingType: ParameterMapping.Attribute
                        //                        }
//                        ParameterMapping {
//                            parameterName: "pos"
//                            shaderVariableName: "pos"
//                            bindingType: ParameterMapping.Attribute
//                        },
                        ,
                        ParameterMapping {
                            parameterName: "vertexId"
                            shaderVariableName: "vertexId"
                            bindingType: ParameterMapping.Attribute
                        }
                    ]
                    shaderProgram: ShaderProgram {

                        //TODO use spheres and modify to accept multiple attributes
                        vertexShaderCode: loadSource("qrc:/SimVis/render/shaders/spheres.vert")
//                                                geometryShaderCode: loadSource("qrc:/SimVis/render/shaders/cylinders.geom")
                        fragmentShaderCode: _fragmentBuilder.finalShader

                        onFragmentShaderCodeChanged: {
                            // console.log(fragmentShaderCode)
                        }
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

                        sourceFile: "qrc:/SimVis/render/shaders/spheres.frag"

                        outputs: [
                            ShaderOutput {
                                id: _fragmentColor
                                type: "vec4"
                                name: "fragColor"
                                value: StandardMaterial {
                                    position: _fragmentBuilder.position
                                    normal: _fragmentBuilder.normal
                                    lights: ShaderGroup {
                                        Nodes.Light {}
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
        id: cylindersMeshInstanced
        primitiveType: GeometryRenderer.TriangleStrip
        enabled: instanceCount != 0
        instanceCount: cylinderData.count

        geometry: PointGeometry {
            attributes: [
                //                Attribute {
                //                    name: "vertex1Position"
                //                    attributeType: Attribute.VertexAttribute
                //                    dataType: Attribute.Float
                //                    dataSize: 3
                //                    byteOffset: 0
                //                    byteStride: 3 + 3 + 1 + 1
                //                    divisor: 1
                //                    buffer: cylinderData.buffer
                //                },
                //                Attribute {
                //                    name: "vertex2Position"
                //                    attributeType: Attribute.VertexAttribute
                //                    dataType: Attribute.Float
                //                    dataSize: 3
                //                    byteOffset: 3
                //                    byteStride: 3 + 3 + 1 + 1
                //                    divisor: 1
                //                    buffer: cylinderData.buffer
                //                },
                //                Attribute {
                //                    name: "radius1"
                //                    attributeType: Attribute.VertexAttribute
                //                    dataType: Attribute.Float
                //                    dataSize: 1
                //                    byteOffset: 6
                //                    byteStride: 3 + 3 + 1 + 1
                //                    divisor: 1
                //                    buffer: cylinderData.buffer
                //                },
                //                Attribute {
                //                    name: "radius2"
                //                    attributeType: Attribute.VertexAttribute
                //                    dataType: Attribute.Float
                //                    dataSize: 1
                //                    byteOffset: 7
                //                    byteStride: 3 + 3 + 1 + 1
                //                    divisor: 1
                //                    buffer: cylinderData.buffer
                //                }
                Attribute {
                    name: "vertex1Position"
                    attributeType: Attribute.VertexAttribute
                    dataType: Attribute.Float
                    dataSize: 3
//                    byteOffset: 7
                    byteStride: 3 + 3 + 1 + 1
                    divisor: 1
                    buffer: cylinderData.buffer
                },
                Attribute {
                    name: "vertex2Position"
                    attributeType: Attribute.VertexAttribute
                    dataType: Attribute.Float
                    dataSize: 3
//                    byteOffset: 7
                    byteStride: 3 + 3 + 1 + 1
                    divisor: 1
                    buffer: cylinderData.buffer
                }
            ]
        }

    }

    components: [
        cylindersMeshInstanced,
        material
    ]
}
