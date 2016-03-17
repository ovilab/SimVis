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
    property alias fragmentColor: _fragmentColor.value
    property alias fragmentBuilder: _fragmentBuilder
    property alias normal: _fragmentBuilder.normal
    property alias position: _fragmentBuilder.position

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
                        vertexShaderCode: loadSource("qrc:/spheres.vert")
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
        id: spheresMeshInstanced
        primitiveType: GeometryRenderer.TriangleStrip
        enabled: instanceCount != 0
        instanceCount: sphereData.count

        geometry: PointGeometry {
            attributes: [
                Attribute {
                    name: "pos"
                    attributeType: Attribute.VertexAttribute
                    dataType: Attribute.Float
                    dataSize: 3
                    divisor: 1
                    buffer: sphereData.buffer
                }
            ]
        }

    }

    components: [
        spheresMeshInstanced,
        material
    ]
}
