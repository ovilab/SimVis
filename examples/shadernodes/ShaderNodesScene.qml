import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

Scene3D {
    id: scene
    aspects: "input"
    Entity {
        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 45
            aspectRatio: 16/9
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d( 0.0, 20.0, -10.0 )
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
                    clearColor: "#777"
                }
            }
        ]

        Entity {
            components: [
                Transform {
                    translation: Qt.vector3d(0.0, 10.0, 0.0)
                },
                PointLight {
                    color: Qt.rgba(1, 1, 1, 1)
                    attenuation: 0.0
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
                    translation: Qt.vector3d(-5, -1, 0)
                }
            ]
        }

        Entity {
            components: [
                PlaneMesh {
                    width: 10
                    height: 10
                    meshResolution: Qt.size(20, 20)
                },
                ShaderBuilderMaterial {
                    vertexPosition: Displacement {
                        vector: Simplex {}
                        strength: 0.2
                    }
                    fragmentColor: StandardMaterial {
                        normal: NormalMap {
                            color: ImageTexture {
                                source: "normal.webp"
                            }
                        }
                        diffuseColor: ImageTexture {
                            source: "diffuse.webp"
                        }
                        lights: ShaderGroup {
                            Nodes.Light {
                                position: Qt.vector3d(0, 10, 0)
                                attenuation: 0.0
                            }
                        }
                    }
                },
                Transform {
                    translation: Qt.vector3d(5, -1, 0)
                }
            ]
        }
    }
}
