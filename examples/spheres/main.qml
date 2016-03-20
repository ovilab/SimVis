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
}

