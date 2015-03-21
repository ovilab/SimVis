import QtQuick 2.2
import QtQuick.Controls 1.1
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Simple harmonic oscillator")

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
        camera: camera
        navigator: navigator

        Camera {
            id: camera
            position: Qt.vector3d(10,10,10)
            fieldOfView: 65
        }

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        MarchingCubes {
            id: points
            numVoxels: Qt.vector3d(100, 100, 100)
            min: Qt.vector3d(-10, -10, -10)
            max: Qt.vector3d(10, 10, 10)
            threshold: 3
        }
    }
}
