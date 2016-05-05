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
            onCameraChanged: {
                camera.position = Qt.vector3d(0.0, 0.0, -2.0)
                camera.upVector = Qt.vector3d(0.0, 1.0, 0.0)
                camera.viewCenter = Qt.vector3d(0.0, 0.0, 0.0)
            }

            sphereData: simulator.sphereData
        }
    }
}
