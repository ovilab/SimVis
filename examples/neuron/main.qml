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
        simulator: reader
        camera: camera

        focus: true

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        Neuron {
            id: neuron
            scale: 0.3
            color: "blue"

            Light {
                id: light
                ambientColor: neuron.color
                specularColor: "white"
                diffuseColor: neuron.color
                ambientIntensity: 0.25
                diffuseIntensity: 1.0
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.0001
                position: Qt.vector3d(10, 10, 10)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }

            Light {
                id: light2
                ambientColor: neuron.color
                specularColor: "white"
                diffuseColor: neuron.color
                ambientIntensity: 0.025
                diffuseIntensity: 1.0
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.0001
                position: Qt.vector3d(-10, -10, 10)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }
        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
    }

    NeuronReader {
        id: reader
        source: "test.xml"
    }

    FileDialog {
        id: fileDialog
        onAccepted: {
            reader.source = fileUrl
        }
    }

    Button {
        text: "Test"
        onClicked: fileDialog.open()
    }
}
