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
        mass: 1.0
        springConstant: 0.5
        dt: 0.001
        ballCount: 1000
    }

    Visualizer {
        id: visualizer
        width: applicationRoot.width
        height: applicationRoot.height - row1.height
        simulator: simulator
        camera: camera
        backgroundColor: "#111111"
        navigator: navigator

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        Spheres {
            id: spheres
            scale: 0.1
            color: "#ff17e6"

            DefaultLight {
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

            DefaultLight {
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

    Rectangle {
        x: 0.5*(parent.width - width)
        height: 50
        width: lightButton.width + simulatorButton.width + renderButton.width + 10
        opacity: 0.9
        color: "white"

        Button {
            id: lightButton
            anchors.left: parent.left
            anchors.leftMargin: 3
            text: "Light"
            onClicked: {
                lightControl.visible = !lightControl.visible
            }
        }

        Button {
            id: simulatorButton
            anchors.left: lightButton.right
            anchors.leftMargin: 3
            text: "Simulator"
            onClicked: simulatorControl.visible = !simulatorControl.visible
        }

        Button {
            id: renderButton
            anchors.left: simulatorButton.right
            anchors.leftMargin: 3
            text: "Rendering"
            onClicked: renderControl.visible = !renderControl.visible
        }

        Label {
            anchors.top: lightButton.bottom
            x: 0.5*(parent.width - width)
            anchors.topMargin: 5
            id: ballsCount
            text: simulator.ballCount+" balls with "+visualizer.fps.toFixed(1) +" fps."
        }
    }

    LightControl {
        id: lightControl
        light: light
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }

    SimulatorControl {
        id: simulatorControl
        simulator: simulator
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }

    RenderControl {
        id: renderControl
        simplexBump: simplexBump
        spheres: spheres
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }

}
