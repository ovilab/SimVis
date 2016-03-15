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

    ShaderBuilderScene {
        id: scene
        anchors.fill: parent
        focus: true
        mix1: slider1.value
        mix2: slider2.value
    }

    Rectangle {
        anchors.fill: column
    }
    Column {
        id: column
        Row {
            Text {
                text: "Noise mix"
            }
            Slider {
                id: slider1
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
                id: slider2
                minimumValue: 0.0
                maximumValue: 1.0
                value: 0.0
            }
        }
    }
}

