import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

import SimVis 1.0
import SimVis.ShaderNodes 1.0

import QtQuick.Scene3D 2.0

import Qt3D.Core 2.0 as Core
import Qt3D.Render 2.0 as Render

Item {
    width: 1650
    height: 900
    visible: true

    ShaderNodesScene {
        id: scene
        anchors.fill: parent
        focus: true
        bumpMix: bumpSlider.value
        colorMix: colorSlider.value
        displacementMix: displacementSlider.value
        blurMix: blurSlider.value
        detailMix: detailSlider.value
        scaleMix: scaleSlider.value
    }

    Rectangle {
        anchors.fill: column
    }
    Column {
        id: column
        Row {
            Text {
                text: "Bump mix"
            }
            Slider {
                id: bumpSlider
                minimumValue: 0.0
                maximumValue: 1.0
                value: 0.5
            }
        }
        Row {
            Text {
                text: "Color mix"
            }
            Slider {
                id: colorSlider
                minimumValue: 0.0
                maximumValue: 1.0
                value: 0.0
            }
        }
        Row {
            Text {
                text: "Displacement mix"
            }
            Slider {
                id: displacementSlider
                minimumValue: 0.0
                maximumValue: 1.0
                value: 0.2
            }
        }
        Row {
            Text {
                text: "Blur mix"
            }
            Slider {
                id: blurSlider
                minimumValue: 0.0
                maximumValue: 0.2
                value: 0.0
            }
        }
        Row {
            Text {
                text: "Noise detail"
            }
            Slider {
                id: detailSlider
                minimumValue: 0.0
                maximumValue: 10.0
                value: 1.0
            }
        }
        Row {
            Text {
                text: "Noise scale"
            }
            Slider {
                id: scaleSlider
                minimumValue: 0.0
                maximumValue: 10.0
                value: 1.0
            }
        }
    }
}

