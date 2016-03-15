import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

Scene3D {
    property alias mix1: mix1.mix
    property alias mix2: mix2.mix
    aspects: "input"
    Entity {
        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 45
            aspectRatio: 16/9
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d( 0.0, 0.0, -20.0 )
            upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
            viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
        }

        Configuration  {
            controlledCamera: camera
        }

        components: [
            FrameGraph {
                id: frameGraph
                activeFrameGraph: ForwardRenderer {
                    camera: camera
                    clearColor: "#012"
                }
            }
        ]

        Entity {

            CylinderMesh {
                id: cylinderMesh
                radius: 2.0
                length: 10.0
                rings: 64
                slices: 32
            }
            ShaderBuilderMaterial {
                id: material
                fragmentColor: StandardMaterial {
                    diffuseColor: "lightblue"
                    specularColor: "white"
                    ambientColor: "white"
                    ambientIntensity: 0.1

                    position: material.position
                    normal: Mix {
                        id: mix1
                        value1: material.normal
                        value2: Simplex {
                            normal: material.normal
                            position: material.position
                        }
                        mix: 0.5
                    }

                    lights: ShaderGroup {
                        Nodes.Light {
                            position: Qt.vector3d(-5.0, 5.0, -5.0)
                        }
                        Nodes.Light {
                            position: Qt.vector3d(-10.0, 2.0, 10.0)
                            color: Mix {
                                id: mix2
                                value1: "steelblue"
                                value2: "purple"
                                mix: 0.0
                            }
                        }
                    }
                }
            }
            components: [
                cylinderMesh,
                material
            ]
        }
    }
}
