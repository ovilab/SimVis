import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import MySimulator 1.0
import QtGraphicalEffects 1.0

ApplicationWindow {
    id: applicationRoot
    title: qsTr("Footballs in a harmonic oscillator - SimVis example")
    width: 800
    height: 600
    visible: true

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        width: applicationRoot.width
        height: applicationRoot.height - row1.height
        simulator: simulator
        Billboards2D {
            id: billboards
            texture: ":/football.png"
            scale: 0.1
        }
    }

    FastBlur {
        id: blurEffect
        anchors.fill: visualizer
        source: visualizer
    }

    Row {
        id: row1
        anchors.top: visualizer.bottom
        spacing: 10

        Slider {
            id: blurRadiusSlider
            minimumValue: 0.0
            maximumValue: 100.0
            value: 0.0
            onValueChanged: blurEffect.radius = value
        }
        Label {
            text: qsTr("Blur radius: %1").arg(blurRadiusSlider.value.toFixed(2))
        }
    }
}
