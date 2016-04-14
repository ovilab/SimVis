import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import MySimulator 1.0

Scene3D {
    aspects: "input"
    Entity {
        Configuration  {
            controlledCamera: frameGraph.camera
        }

        components: [
            StandardFrameGraph {
                id: frameGraph
                camera.position: Qt.vector3d(0.0, 0.0, 4.0)
            }
        ]

        FunctionSurfaceEntity {
            id: fsEntity
            camera: frameGraph.camera
            sphereData: simulator.sphereData
            fragmentColor: StandardMaterial {
                diffuseColor: "steelblue"
                specularColor: "white"
                position: fsEntity.fragmentBuilder.position
                normal: fsEntity.fragmentBuilder.normal
                lights: ShaderGroup {
                    Nodes.Light {
                        position: frameGraph.camera.position
                        color: "red"
                        attenuation: 0.01
                    }
                }
            }
        }

        MySimulator {
            id: simulator
            dt: 0.01
        }
    }
}