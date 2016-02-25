import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import CylinderSimulator 1.0
import SimVis 1.0

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
            visible: true
            scale: 1.0
            radius: 1.2

            Light {
                id: light2
                ambientColor: "blue"
                specularColor: "white"
                diffuseColor: "cyan"
                ambientIntensity: 0.1
                diffuseIntensity: 1.0
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.001
                position: Qt.vector3d(0.0, 20.0, -20.0)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }

            SimplexBump {
                id: simplexBump
                enabled: true
                intensity: 0.03
                scale: 5.0
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
