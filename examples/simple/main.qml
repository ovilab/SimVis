import QtQuick 2.1
import SimpleSimulator 1.0
import SimVis 1.0

Rectangle {
    visible: true
    width: 640
    height: 480
//    title: qsTr("Simple harmonic oscillator")

    SimpleSimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
//        Points {
//            id: points
//            pointSize: 10.0
//        }
        Spheres {
            id: spheres
            visible: true
            color: "#ff17e6"
            scale: 0.4

            Light {
                id: light
                ambientColor: "yellow"
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.1
                diffuseIntensity: 1.4
                specularIntensity: 2.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(12.5, 0.5, 0.5)
            }
        }
    }
}
