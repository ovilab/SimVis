import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import SphereSimulator 1.0

Scene3D {
    aspects: "input"
    Visualizer {
        id: visualizer
        SphereSimulator {
            id: simulator
            dt: 0.01
        }
        Spheres {
            id: spheresEntity
            camera: visualizer.camera
            sphereData: simulator.sphereData
            fragmentColor: StandardMaterial {
                color: "steelblue"
                hardness: 0.05
                position: spheresEntity.fragmentBuilder.position
                normal: Add {
                    value1: spheresEntity.normal
                    value2: Simplex {
                        vector: spheresEntity.fragmentBuilder.normal
                        scale: 2.5
                        strength: 0.3
                    }
                }
                lights: ShaderGroup {
                    Nodes.Light {
                        position: Qt.vector3d(-20.0, 0.0, 0.0)
                        color: "red"
                        attenuation: 0.01
                    }
                    Nodes.Light {
                        position: Qt.vector3d(20.0, 0.0, 0.0)
                        color: "yellow"
                        attenuation: 0.01
                    }
                    Nodes.Light {
                        position: Qt.vector3d(0.0, 0.0, -20.0)
                        color: "white"
                        attenuation: 0.004
                    }
                    Nodes.Light {
                        position: Qt.vector3d(0.0, 55.0, -20.0)
                        color: "brown"
                        attenuation: 0.004
                    }
                    Nodes.Light {
                        position: Qt.vector3d(29.0, 29.0, -53.0)
                        color: "yellow"
                        attenuation: 0.004
                    }
                    Nodes.Light {
                        position: frameGraph.camera.position
                        color: "red"
                        attenuation: 0.01
                    }
                }
            }
        }
    }
}
