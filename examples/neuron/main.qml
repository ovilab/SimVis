import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import NeuroML 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 1280
    height: 720
    visible: true

    Visualizer {
        anchors.fill: parent
        simulator: NeuronReader {
            id: reader
            source: "test.xml"
        }
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

            fragColor: Diffuse {
                position: cylinders.shader.position
                color: Qt.vector3d(1.0, 1.0, 1.0)
                specularIntensity: 0.4
                normal: cylinders.shader.normal

                lights: ShaderGroup {
                    Light {
                        position: Qt.vector3d(50.0, 50.0, -50.0)
                        strength: 0.3
                    }
                    Light {
                        position: Qt.vector3d(50.0, -50.0, 50.0)
                        strength: 0.3
                    }
                    Light {
                        position: camera.position
                        strength: 0.3
                    }
                }
            }
        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
        farPlane: 5000.0
        position: Qt.vector3d(0.0, 0.0, -70.0)
        upVector: Qt.vector3d(0.0, -1.0, 0.0)
        viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
    }
}
