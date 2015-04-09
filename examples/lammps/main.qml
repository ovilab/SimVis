import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    id: applicationRoot
    title: qsTr("Footballs in a harmonic oscillator - SimVis example")
    width: 1650
    height: 900
    visible: true

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        width: applicationRoot.width
        height: applicationRoot.height - row1.height
        simulator: simulator
        camera: camera
        backgroundColor: "#fff"
        navigator: navigator

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        Spheres {
            id: spheres
            visible: true
            scale: 0.1
            color: "#ff17e6"

            Light {
                id: light
                ambientColor: spheres.color
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.025
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(0.5, 0.5, 0.5)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }

            Light {
                id: light2
                ambientColor: spheres.color
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.025
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(Math.sin(0.1*6.28*visualizer.time), Math.cos(0.1*6.28*visualizer.time), 0)
            }
        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
    }

    FastBlur {
        id: blurEffect
        anchors.fill: visualizer
        source: visualizer
    }

    Row {
        id: row1
        anchors.top: visualizer.bottom
        spacing: 10

        Slider {
            id: blurRadiusSlider
            minimumValue: 0.0
            maximumValue: 100.0
            value: 0.0
            onValueChanged: blurEffect.radius = value
        }
        Label {
            text: qsTr("Blur radius: %1").arg(blurRadiusSlider.value.toFixed(2))
        }
    }
}
