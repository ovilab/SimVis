import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0
import MySimulator 1.0

Rectangle {
    id: renderControlRoot
    width: 320
    property SimplexBump simplexBump
    property Spheres spheres
    height: spheresGroup.height + bump.height + okButton.height + 10
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    GroupBox {
        id: spheresGroup
        anchors.top: parent.top
        width: parent.width
        title: qsTr("Spheres")


        Label {
            id: sizeLabel
            text: "Size"
        }

        Slider {
            id: size
            anchors.left: sizeLabel.right
            anchors.leftMargin: 5
            minimumValue: 0.01
            maximumValue: 0.25
            value: spheres.scale
            onValueChanged: {
                if(spheres != null) {
                    spheres.scale = value
                    sizeLabel.text = "Size ("+spheres.scale.toFixed(2)+")"
                }
            }
        }
    }

    GroupBox {
        id: bump
        anchors.top: spheresGroup.bottom
        width: parent.width
        title: qsTr("Simplex bump")

        CheckBox {
            id: bumpEnabled
            text: "Enabled"
            checked: simplexBump.enabled
            onCheckedChanged: {
                if(simplexBump != null) {
                    simplexBump.enabled = checked
                }
            }
        }

        Label {
            id: strengthLabel
            anchors.top: bumpEnabled.bottom
            text: qsTr("Strength")
        }

        Slider {
            id: bumpIntensity
            anchors.top: strengthLabel.top
            anchors.left: strengthLabel.right
            anchors.leftMargin: 5
            minimumValue: 0.0
            maximumValue: 0.5
            value: simplexBump.intensity
            onValueChanged: simplexBump.intensity = value
        }


        Label {
            id: scaleLabel
            anchors.top: strengthLabel.bottom
            width: strengthLabel.width
            text: qsTr("Scale")
        }

        Slider {
            id: bumpScale
            anchors.top: scaleLabel.top
            anchors.left: scaleLabel.right
            anchors.leftMargin: 5
            minimumValue: 0.0
            maximumValue: 10.0
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

