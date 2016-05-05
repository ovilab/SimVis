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

    FunctionSurfaceScene {
        id: scene
        anchors.fill: parent
        focus: true
        r: sliderR.value
        nearPlane: sliderNP.value
        farPlane: sliderFP.value
    }

    Column {
        Row {
            Slider {
                id: sliderR
                minimumValue: 0
                maximumValue: 10
                stepSize: 0.1
                value: scene.r
            }
            Label {
                text: sliderR.value
            }
        }
        Row {

            Slider {
                id: sliderNP
                minimumValue: 1
                maximumValue: 30
                stepSize: 1
                value: scene.nearPlane
            }
            Label {
                text: sliderNP.value
            }
        }
        Row {
            Slider {
                id: sliderFP
                minimumValue: 100
                maximumValue: 10000
                stepSize: 100
                value: scene.farPlane
            }
            Label {
                text: sliderFP.value
            }
        }
    }
}
