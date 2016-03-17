import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0
import QtQuick.Scene3D 2.0
import QtMultimedia 5.5

Item {
    id: applicationRoot
//    title: qsTr("Spheres in an harmonic oscillator - SimVis example")
    width: 1650
    height: 900
    visible: true

    SpheresScene {
        id: scene
        anchors.fill: parent
        focus: true
        variable: slider.value
    }

    Slider {
        id: slider
        minimumValue: 0.0
        maximumValue: 1.0
    }
    Text {
//        text: scene.rotation
    }
    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        onMouseXChanged: {
            scene.chest.translation.x = mouse.x / 1000
            mouse.accepted = false
        }
        onMouseYChanged: {
            scene.chest.translation.y = mouse.y / 1000
            mouse.accepted = false
        }
    }
}

