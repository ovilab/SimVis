import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0

Rectangle {
    id: lightControlRoot
    width: 320
    height: sliders.height + okButton.height + 10
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    GroupBox {
        id: sliders
        width: parent.width
        height: 120
        title: qsTr("Light")

        Label {
            id: ambient
            width: shininess.width
            text: "Ambient"
        }

        Slider {
            id: ambientIntensity
            anchors.left: ambient.right
            anchors.leftMargin: 5
            maximumValue: 1
            minimumValue: 0
            value: light.ambientIntensity
            onValueChanged: light.ambientIntensity = value
        }

        Label {
            id: diffuse
            width: shininess.width
            anchors.top: ambient.bottom
            text: "Diffuse"
        }

        Slider {
            id: diffuseIntensity
            anchors.top: diffuse.top
            anchors.left: diffuse.right
            anchors.leftMargin: 5
            maximumValue: 1.0
            minimumValue: 0.0
            value: light.diffuseIntensity
            onValueChanged: light.diffuseIntensity = value
        }

        Label {
            id: specular
            width: shininess.width
            anchors.top: diffuse.bottom
            text: "Specular"
        }

        Slider {
            id: specularIntensity
            anchors.top: specular.top
            anchors.left: specular.right
            anchors.leftMargin: 5
            maximumValue: 1.0
            minimumValue: 0.0
            value: light.specularIntensity
            onValueChanged: light.specularIntensity = value
        }

        Label {
            id: shininess
            anchors.top: specular.bottom
            text: "Shininess"
        }

        Slider {
            id: shininessIntensity
            anchors.top: shininess.top
            anchors.left: shininess.right
            anchors.leftMargin: 5
            height: 22
            maximumValue: 50
            minimumValue: 0.0
            value: light.shininess
            onValueChanged: light.shininess = value
        }
    }

    Button {
        id: okButton
        x: parent.width*0.5 - width*0.5
        anchors.top: sliders.bottom
        anchors.topMargin: 5
        text: qsTr("OK")
        onClicked: lightControlRoot.visible = false
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            visible = false
        }
    }
}
