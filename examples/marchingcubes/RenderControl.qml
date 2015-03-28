import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0
import MySimulator 1.0

Rectangle {
    id: renderControlRoot
    width: 320
    height: renderingMode.height + texture.height + bump.height + okButton.height + 10
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    GroupBox {
        id: renderingMode
        width: parent.width
        height: 66
        title: qsTr("Rendering mode")

        Button {
            id: rendering
            text: "Lines"
            onClicked: {
                marchingCubes.mode = MarchingCubes.LINES
            }
        }

        Button {
            id: rendering1
            anchors.left: rendering.right
            text: "Triangles"
            onClicked: {
                marchingCubes.mode = MarchingCubes.FRONT_AND_BACK
            }
        }
    }

    GroupBox {
        id: texture
        anchors.top: renderingMode.bottom
        width: parent.width
        height: renderingMode.height
        title: qsTr("Texture")

        Button {
            id: textureNone
            text: "None"
            onClicked: {
                simplexTexture.enabled = false
                simplexTexture.timeDependent = false
            }
        }

        Button {
            id: textureSimplex
            anchors.left: textureNone.right
            text: "Simplex"
            onClicked: {
                simplexTexture.enabled = true
                simplexTexture.timeDependent = false
            }
        }

        Button {
            id: textureSimplexTime
            anchors.left: textureSimplex.right
            text: "Simplex(t)"
            onClicked: {
                simplexTexture.enabled = true
                simplexTexture.timeDependent = true
            }
        }
    }

    GroupBox {
        id: bump
        anchors.top: texture.bottom
        width: parent.width
        height: 82
        title: qsTr("Bump map")


        Label {
            id: strengthLabel
            x: 0
            y: -2
            text: qsTr("Strength")
        }

        Slider {
            id: bumpIntensity
            anchors.left: strengthLabel.right
            anchors.leftMargin: 5
            minimumValue: 0.0
            maximumValue: 0.5
            value: simplexBump.intensity
            onValueChanged: simplexBump.intensity = value
        }


        Label {
            id: scaleLabel
            x: 0
            y: 20
            width: strengthLabel.width
            text: qsTr("Scale")
        }

        Slider {
            id: bumpScale
            anchors.left: scaleLabel.right
            anchors.leftMargin: 5
            y: 20
            minimumValue: 0.0
            maximumValue: 5.0
            value: simplexBump.scale
            onValueChanged: simplexBump.scale = value
        }
    }

    Button {
        id: okButton
        x: parent.width*0.5 - width*0.5
        anchors.top: bump.bottom
        anchors.topMargin: 5
        text: qsTr("OK")
        onClicked: renderControlRoot.visible = false
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            visible = false
        }
    }
}

