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

import Qt3D.Core 2.0 as Core
import Qt3D.Render 2.0 as Render

Item {
    id: applicationRoot
//    title: qsTr("Spheres in an harmonic oscillator - SimVis example")
    width: 1650
    height: 900
    visible: true

    SpheresScene {
        anchors.fill: parent
        focus: true
    }
}

