import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0
import MySimulator 1.0

Rectangle {
    id: renderControlRoot
    width: 300
    height: 220
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    GroupBox {
        id: renderingMode
        x: 0
        y: 0
        width: 300
        height: 49
        title: qsTr("Rendering mode")

        Button {
            id: rendering
            x: 0
            y: -4
            text: "Lines"
            onClicked: {
                marchingCubes.mode = MarchingCubes.LINES
            }
        }

        Button {
            id: rendering1
            x: 73
            y: -4
            text: "Triangles"
            onClicked: {
                marchingCubes.mode = MarchingCubes.FRONT_AND_BACK
            }
        }
    }

    GroupBox {
        id: texture
        x: 0
        y: 55
        width: 300
        height: 53
        title: qsTr("Texture")

        Button {
            id: textureNone
            x: 0
            y: -4
            text: "None"
            onClicked: {
                simplexTexture.enabled = false
                simplexTexture.timeDependent = false
            }
        }

        Button {
            id: textureSimplex
            x: 74
            y: -4
            text: "Simplex"
            onClicked: {
                simplexTexture.enabled = true
                simplexTexture.timeDependent = false
            }
        }

        Button {
            id: textureSimplexTime
            x: 164
            y: -4
            text: "Time Simplex"
            onClicked: {
                simplexTexture.enabled = true
                simplexTexture.timeDependent = true
            }
        }
    }

    Button {
        id: button1
        x: 123
        y: 186
        text: qsTr("OK")
        onClicked: renderControlRoot.visible = false
    }

    GroupBox {
        id: bump
        x: 0
        y: 114
        width: 300
        height: 66
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
            minimumValue: 0
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
            y: 20
            minimumValue: 0.1
            maximumValue: 5
            value: simplexBump.scale
            onValueChanged: simplexBump.scale = value
        }
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            visible = false
        }
    }
}

