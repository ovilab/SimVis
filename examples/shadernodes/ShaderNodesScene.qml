import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

Scene3D {
    property real bumpMix: 0.5
    property real colorMix: 0.0
    property real displacementMix: 0.0
    property real blurMix: 0.0
    property real scaleMix: 0.0
    property real detailMix: 0.0
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
            SphereMesh {
                id: mesh
                radius: 4.0
                rings: 64
                slices: 32
            }
            ShaderBuilderMaterial {
                id: material
//                vertexPosition: Displacement {
//                    vector: Noise {
//                        scale: scaleMix
//                        detail: detailMix
//                        vector: material.vertex.position
//                    }
//                    strength: displacementMix
//                }

                fragmentColor: StandardMaterial {
                    diffuseColor: BumpNode {
                        height: ImageTexture {
                            id: image
                            source: "diffuse.webp"
                        }
                    }
                    ambientIntensity: 0.1
                    ambientColor: diffuseColor

//                    normal: BumpNode {
//                        delegate: Add {
//                            value1: Simplex {}
//                            value2: Simplex {}
//                        }
//                        strength: bumpMix
//                    }

                    lights: ShaderGroup {
                        Nodes.Light {
                            position: Qt.vector3d(-5.0, 0.0, -5.0)
                            attenuation: 0.001
                        }
                        Nodes.Light {
                            position: Qt.vector3d(5.0, 0.0, -5.0)
                            attenuation: 0.001
                        }
                        Nodes.Light {
                            position: Qt.vector3d(-10.0, 2.0, 10.0)
                            color: Mix {
                                value1: "steelblue"
                                value2: "purple"
                                mix: colorMix
                            }
                        }
                    }
                }
            }
            components: [
                mesh,
                material
            ]
        }
    }
}
