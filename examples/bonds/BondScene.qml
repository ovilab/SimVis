import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import BondSimulator 1.0

Scene3D {
    aspects: "input"
    Visualizer {
        id: visualizer
        clearColor: "#dde"
        BondSimulator {
            id: simulator
            dt: 0.01
        }
        Spheres {
            id: spheresEntity
            camera: visualizer.camera
            sphereData: simulator.sphereData
        }
        Cylinders {
            cylinderData: simulator.cylinderData
            fragmentColor: StandardMaterial {
                lights: ShaderGroup {
                    Light {
                        position: Qt.vector3d(5, 5, 5)
                    }
                }
            }
        }
    }
}
