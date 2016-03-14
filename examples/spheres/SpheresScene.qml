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
            }
        ]

        SpheresEntity {
            camera: frameGraph.camera
            sphereData: simulator.sphereData
        }

        MySimulator {
            id: simulator
            dt: 0.001
        }

        Chest {}
    }
}
