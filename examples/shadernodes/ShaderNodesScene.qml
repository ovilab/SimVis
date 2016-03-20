import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

Scene3D {
    id: scene
    property real bumpMix: 0.5
    property real colorMix: 0.0
    property real displacementMix: 0.0
    property real blurMix: 0.0
    property real scaleMix: 0.0
    property real detailMix: 0.0
    property real bumpDistance: 0.0
    property real bumpStrength: 0.0
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
            components: [
                SphereMesh {
                    radius: 2
                },
                Transform {
                    translation: Qt.vector3d(10.0, 10.0, 10.0)
                },
                PointLight {
                    color: Qt.rgba(1, 1, 1, 1)
                }
            ]
        }

        Entity {
            components: [
                PlaneMesh {
                    width: 10
                    height: 10
                },
                NormalDiffuseMapMaterial {
                    diffuse: "diffuse.webp"
                    normal: "normal.webp"
                    ambient: Qt.rgba(0, 0, 0, 1)
                },
                Transform {
                    translation: Qt.vector3d(10, -1, 0)
                }
            ]
        }

        Entity {
//            CylinderMesh {
//                id: mesh
//                radius: 6.0
//                rings: 64
//                slices: 32
//            }
            PlaneMesh {
                id: mesh
                width: 10
                height: 10
//                meshResolution: Qt.size(20, 20)
            }
//            NormalDiffuseMapMaterial {
//                id: material
//                diffuse: "diffuse.webp"
//                normal: "normal.webp"
//            }

            ShaderBuilderMaterial {
                id: material

                fragmentColor: StandardMaterial {
                    normal: NormalMap {
                        color: ImageTexture {
                            source: "normal.webp"
                        }
                    }

                    diffuseColor: ImageTexture {
                        source: "diffuse.webp"
                    }

                    ambientIntensity: 0.01
                    ambientColor: diffuseColor

                    diffuseIntensity: 3.0

                    specularIntensity: 1.0
                    hardness: 160.0

                    lights: ShaderGroup {
                        Nodes.Light {
                            position: Qt.vector3d(10, 10, 10)
                            attenuation: 0.0
                        }
                    }
                }
            }
            Transform {
                id: transofmr
                translation: Qt.vector3d(0, -1, 0)
            }

            components: [
                mesh,
                material,
                transofmr
            ]
        }
    }
}
