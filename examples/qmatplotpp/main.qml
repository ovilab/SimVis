import QtQuick 2.2
import QtQuick.Controls 1.1
import SimpleSimulator 1.0
import SimVis 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Simple harmonic oscillator")

    SimpleSimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
//        Points {
//            id: points
//        }

        Plotting {
            id: plotting
        }
    }
}
