import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2

import SimVis 1.0
import MySimulator 1.0

Entity {
    id: spheresRoot
    property var simulator
    Material {
        id: spheresGeometryShaderTechnique
        effect: Effect {
            techniques: Technique {
                graphicsApiFilter {
                    api: GraphicsApiFilter.OpenGL
                    profile: GraphicsApiFilter.CoreProfile
                    minorVersion: 3
                    majorVersion: 3
                }
                renderPasses: RenderPass {
                    bindings: [
                        ParameterMapping {
                            parameterName: "pos"
                            shaderVariableName: "pos"
                            bindingType: ParameterMapping.Attribute
                        }
                    ]
                    shaderProgram: ShaderProgram {
//                        vertexShaderCode: loadSource("qrc:/instanced.vert")
//                        geometryShaderCode: loadSource("qrc:/instanced.geom")
//                        fragmentShaderCode: loadSource("qrc:/instanced.frag")

                        vertexShaderCode: loadSource("qrc:/regular.vert")
                        fragmentShaderCode: loadSource("qrc:/regular.frag")
                    }
                }
            }
        }
    }
    GeometryRenderer {
        id: cylinderMeshInstanced
        enabled: instanceCount != 0
        //        primitiveType: GeometryRenderer.Points
        instanceCount: 20000 // TODO make instanceCount depend on buffer/simulator
//        geometry: PointGeometry {
//            attributes: [
//                instanceDataAttribute
//            ]
//        }
        geometry: SphereGeometry {
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
            buffer: simulator.positionBuffer
        }
    }
    Entity {
        components: [
            cylinderMeshInstanced,
            spheresGeometryShaderTechnique
        ]
    }
}
