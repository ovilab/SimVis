import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    id: applicationRoot
    title: qsTr("Footballs in a harmonic oscillator - SimVis example")
    width: 1920
    height: 1080
    visible: true

    MySimulator {
        id: simulator
        mass: 1.0
        springConstant: 1.0
        dt: 0.01
    }

    Visualizer {
        id: visualizer
        width: applicationRoot.width - 350
        height: applicationRoot.height - row1.height
        simulator: simulator
        Billboards2D {
            id: billboards
            visible: billboardsVisible.checked
            texture: ":/football.png"
            scale: 0.1
        }

        Points2D {
            id: points
            visible: pointsVisible.checked
            pointSize: 10.0
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

    Item {
        anchors.left: visualizer.right
        Row {
            id: controlsRow1
            Label {
                text: "Mass: "
            }
            Slider {
                id: massSlider
                minimumValue: 0.1
                maximumValue: 10.0
                value: 1.0
                onValueChanged: simulator.mass = value
            }
        }

        Row {
            id: controlsRow2
            anchors.top: controlsRow1.bottom

            Label {
                text: "Spring constant: "
            }

            Slider {
                id: springSlider
                minimumValue: 0.1
                maximumValue: 10.0
                value: 1.0
                onValueChanged: simulator.springConstant = value
            }
        }

        Row {
            id: controlsRow3
            anchors.top: controlsRow2.bottom

            CheckBox {
                id: billboardsVisible
                text: "Billboards"
                checked: true
            }

            CheckBox {
                id: pointsVisible
                text: "Points"
                checked: false
            }
        }

        Row {
            id: controlsRow4
            anchors.top: controlsRow3.bottom

            TextField {
                text: simulator.numberOfBalls
                inputMethodHints: Qt.ImhFormattedNumbersOnly
                onTextChanged: {
                    var value = Math.round(parseInt(text))
                    simulator.setNumberOfBalls(text)
                    simulator.reset()
                }
            }

            Button {
                text: "Reset"
                onClicked: simulator.reset();
            }
        }
    }
}
