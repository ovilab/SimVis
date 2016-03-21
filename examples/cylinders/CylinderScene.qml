import SimVis 1.0
import SimVis.ShaderNodes 1.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import CylinderSimulator 1.0

Scene3D {
    aspects: "input"
    Visualizer {
        CylinderSimulator {
            id: simulator
        }

        Cylinders {
            id: cylinders
            cylinderData: simulator.cylinderData
            sphereData: simulator.sphereData
        }
    }

//    Camera {
//        id: camera
//        nearPlane: 0.1
//        farPlane: 5000.0
//        position: Qt.vector3d(0.0, 0.0, -20.0)
//        upVector: Qt.vector3d(0.0, -1.0, 0.0)
//        viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
//    }
}
