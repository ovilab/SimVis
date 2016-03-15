import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import MySimulator 1.0

Scene3D {
    aspects: "input"
    property alias variable: spheresEntity.variable
    Entity {
        Configuration  {
            controlledCamera: frameGraph.camera
        }

        components: [
            StandardFrameGraph {
                id: frameGraph
                camera.position: Qt.vector3d(0.0, 0.0, 1000.0)
            }
        ]

        SpheresEntity {
            id: spheresEntity
            camera: frameGraph.camera
            sphereData: simulator.sphereData
        }

        MySimulator {
            id: simulator
            dt: 0.1
        }

//        Chest {}
    }
}
