import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import CylinderSimulator 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

ApplicationWindow {
    title: qsTr("Cylinders")
    width: 1280
    height: 720
    visible: true

    Visualizer {
        anchors.fill: parent
        simulator: CylinderSimulator {}
        camera: camera

        focus: true

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        Cylinders {
            id: cylinders
            visible: true
            scale: 1.0
            radius: 1.2
            fragColor: StandardMaterial {
                position: cylinders.shader.position
                color: Qt.vector3d(0.6, 0.8, 1.0)
                specularIntensity: 1.0
                normal: Simplex {
                    normal: cylinders.shader.normal
                    position: cylinders.shader.position
                    scale: 8.0
                    strength: 0.2
                }

                lights: ShaderGroup {
                    Light {
                        position: Qt.vector3d(5.0, 5.0, 0.0)
                        color: "steelblue"
                    }
                    Light {
                        position: Qt.vector3d(-5.0, 5.0, 0.0)
                        color: "white"
                    }
                }
            }
        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
        farPlane: 5000.0
        position: Qt.vector3d(0.0, 0.0, -20.0)
        upVector: Qt.vector3d(0.0, -1.0, 0.0)
        viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
    }
}
