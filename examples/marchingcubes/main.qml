import QtQuick 2.2
import QtQuick.Controls 1.1
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Marching cubes demo")

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        width: parent.width
        height: parent.height - row1.height
        simulator: simulator
        camera: camera
        navigator: navigator
        focus: true

        Camera {
            id: camera
            position: Qt.vector3d(10,0,10)
            fieldOfView: 65
        }

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

        MarchingCubes {
            id: marchingCubes
            numVoxels: Qt.vector3d(50, 50, 50)
            min: Qt.vector3d(-Math.PI, -Math.PI, -Math.PI)
            max: Qt.vector3d(Math.PI, Math.PI, Math.PI)
            threshold: 0.0
            color: "green"
            // mode: MarchingCubes.FRONT
        }

        Keys.onPressed: {
            if(event.key === Qt.Key_1) {
                console.log("Front and back")
                marchingCubes.mode = MarchingCubes.FRONT_AND_BACK
            } else if(event.key === Qt.Key_2) {
                console.log("Front")
                marchingCubes.mode = MarchingCubes.FRONT
            } else if(event.key === Qt.Key_3) {
                console.log("Back")
                marchingCubes.mode = MarchingCubes.BACK
            } else if(event.key === Qt.Key_4) {
                console.log("Lines")
                marchingCubes.mode = MarchingCubes.LINES
            }
        }
    }

    Row {
        id: row1
        anchors.top: visualizer.bottom
        spacing: 10

        Slider {
            id: alphaSlider
            minimumValue: 0.0
            maximumValue: 1.0
            value: 1.0
            onValueChanged: marchingCubes.alpha = value
        }
        Label {
            text: qsTr("Alpha: %1").arg(alphaSlider.value.toFixed(2))
        }
    }
}
