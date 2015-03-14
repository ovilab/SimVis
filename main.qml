import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import MySimulator 1.0
ApplicationWindow {
    title: qsTr("Hello World")
    width: 800
    height: 600
    visible: true

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
        Billboards2D {
            id: billboards
            texture: ":/football.png"
            scale: 0.1
        }
    }
}
