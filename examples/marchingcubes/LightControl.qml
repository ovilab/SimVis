import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0

Rectangle {
    id: lightControlRoot
    width: 300
    height: 130
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    Label {
        id: ambient
        x: 20
        y: 8
        text: "Ambient"
    }

    Slider {
        id: ambientIntensity
        x: 81
        y: 5
        maximumValue: 1
        minimumValue: 0
        value: light.ambientIntensity
        onValueChanged: light.ambientIntensity = value
    }

    Label {
        id: ambient1
        x: 20
        y: 30
        text: "Diffuse"
    }

    Slider {
        id: diffuseIntensity
        x: 81
        y: 27
        maximumValue: 1
        minimumValue: 0
        value: light.diffuseIntensity
        onValueChanged: light.diffuseIntensity = value
    }

    Label {
        id: ambient2
        x: 20
        y: 52
        text: "Specular"
    }

    Slider {
        id: specularIntensity
        x: 81
        y: 49
        maximumValue: 1
        minimumValue: 0
        value: light.specularIntensity
        onValueChanged: light.specularIntensity = value
    }

    Label {
        id: shininess
        x: 20
        y: 74
        text: "Shininess"

    }

    Slider {
        id: specularIntensity1
        x: 81
        y: 71
        height: 22
        maximumValue: 50
        minimumValue: 0
        value: light.shininess
        onValueChanged: light.shininess = value
    }

    Button {
        id: button1
        x: 123
        y: 99
        text: qsTr("OK")
        onClicked: lightControlRoot.visible = false
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            visible = false
        }
    }
}

