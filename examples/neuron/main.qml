import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import NeuroML 1.0
import SimVis 1.0

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
        backgroundColor: "blue"

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
                ambientColor: "yellow"
                specularColor: "white"
                diffuseColor: "#ff17e6"
                ambientIntensity: 0.5
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(0.5, 0.5, 0.5)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }
        }

//        Spheres {
//            id: spheres
//            visible: true
//            scale: 1.0
//            color: "#ff17e6"

//            Light {
//                id: light
//                ambientColor: "yellow"
//                specularColor: "white"
//                diffuseColor: spheres.color
//                ambientIntensity: 0.5
//                diffuseIntensity: 0.5
//                specularIntensity: 1.0
//                shininess: 40.0
//                attenuation: 0.01
//                position: Qt.vector3d(0.5, 0.5, 0.5)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
//            }
//        }

//        Neuron {
//            id: neuron
//            scale: 0.3
//            color: "blue"

//            Light {
//                id: light
//                ambientColor: neuron.color
//                specularColor: "white"
//                diffuseColor: neuron.color
//                ambientIntensity: 0.25
//                diffuseIntensity: 1.0
//                specularIntensity: 1.0
//                shininess: 40.0
//                attenuation: 0.0001
//                position: Qt.vector3d(10, 10, 10)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
//            }

//            Light {
//                id: light2
//                ambientColor: neuron.color
//                specularColor: "white"
//                diffuseColor: neuron.color
//                ambientIntensity: 0.025
//                diffuseIntensity: 1.0
//                specularIntensity: 1.0
//                shininess: 40.0
//                attenuation: 0.0001
//                position: Qt.vector3d(-10, -10, 10)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
//            }
//        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
        farPlane: 5000.0
        position: Qt.vector3d(1.0, 1.0, 10.0)
        upVector: Qt.vector3d(0.0, -1.0, 0.0)
        viewCenter: Qt.vector3d(2.0, 2.0, 0.0)
    }

//    FileDialog {
//        id: fileDialog
//        onAccepted: {
//            reader.source = fileUrl
//        }
//    }

//    Button {
//        text: "Test"
//        onClicked: fileDialog.open()
//    }
}
