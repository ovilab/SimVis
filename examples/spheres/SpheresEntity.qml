import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2

import SimVis 1.0 as SimVis
import MySimulator 1.0

Entity {
    id: spheresRoot
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
        effect: Effect {
            techniques: Technique {
                renderPasses: RenderPass {
                    bindings: [
                        ParameterMapping {
                            parameterName: "pos"
                            shaderVariableName: "pos"
                            bindingType: ParameterMapping.Attribute
                        }
//                        ParameterMapping {
//                            parameterName: "viewVector"
//                            shaderVariableName: "viewVector"
//                            bindingType: ParameterMapping.Uniform
//                        },
//                        ParameterMapping {
//                            parameterName: "upVector"
//                            shaderVariableName: "upVector"
//                            bindingType: ParameterMapping.Uniform
//                        },
//                        ParameterMapping {
//                            parameterName: "rightVector"
//                            shaderVariableName: "rightVector"
//                            bindingType: ParameterMapping.Uniform
//                        }
                    ]
                    shaderProgram: ShaderProgram {
                        vertexShaderCode: loadSource("qrc:/instanced.vert")
                        geometryShaderCode: loadSource("qrc:/instanced.geom")
                        fragmentShaderCode: loadSource("qrc:/instanced.frag")
                    }
                }
            }
        }
    }
    GeometryRenderer {
        id: cylinderMeshInstanced
        primitiveType: GeometryRenderer.Points
        enabled: instanceCount != 0
        instanceCount: sphereData.count

        geometry: PointGeometry {
            attributes: [
                instanceDataAttribute
            ]
        }

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

    Entity {
        components: [
            cylinderMeshInstanced,
            spheresGeometryShaderTechnique
        ]
    }
}
