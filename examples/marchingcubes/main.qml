import QtQuick 2.2
import QtQuick.Controls 1.1
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    visible: true
    width: 1600
    height: 900
    title: qsTr("Marching cubes demo")

    MySimulator {
        id: simulator
        continuousScalarField: true
    }

    Visualizer {
        id: visualizer
        width: parent.width
        height: parent.height - row1.height - row2.height
        simulator: simulator
        camera: camera
        navigator: navigator
        focus: true

        Camera {
            id: camera
            position: Qt.vector3d(0, 10, 0)
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
            lightPosition: camera.position
            color: "green"
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
            } else if(event.key === Qt.Key_5) {
                simulator.continuousScalarField = !simulator.continuousScalarField;
            }
        }
    }

    Row {
        id: row1
        anchors.top: visualizer.bottom
        spacing: 10

        Slider {
            id: scaleSlider
            minimumValue: 0.1
            maximumValue: 5.0
            value: 1.0
            onValueChanged: marchingCubes.scale = value
        }
        Label {
            text: qsTr("Scale: %1").arg(scaleSlider.value.toFixed(2))
        }
    }

    Row {
        id: row2
        anchors.top: row1.bottom
        spacing: 10

        Label {
            text: "Nx:"
        }

        TextField {
            id: numVoxelsX
            text: marchingCubes.numVoxels.x
        }

        Label {
            text: "Ny:"
        }

        TextField {
            id: numVoxelsY
            text: marchingCubes.numVoxels.y
        }

        Label {
            text: "Nz:"
        }

        TextField {
            id: numVoxelsZ
            text: marchingCubes.numVoxels.z
        }

        Button {
            id: sinus
            text: "Sinus"
            onClicked: {
                simulator.geometry = MySimulator.SINUS
                marchingCubes.threshold = 0
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
            }
        }
        Button {
            id: sphere
            text: "Sphere"
            onClicked: {
                simulator.geometry = MySimulator.SPHERE
                marchingCubes.threshold = 3
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
            }
        }
        Button {
            id: cube
            text: "Cube"
            onClicked: {
                simulator.geometry = MySimulator.CUBE
                marchingCubes.threshold = 3
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
            }
        }

        Button {
            id: perlin
            text: "Perlin"
            onClicked: {
                simulator.geometry = MySimulator.PERLIN
                marchingCubes.threshold = 0.5
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
            }
        }
    }
}
