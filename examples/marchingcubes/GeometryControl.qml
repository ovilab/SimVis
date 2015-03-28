import QtQuick 2.2
import QtQuick.Controls 1.1
import SimVis 1.0
import MySimulator 1.0

Rectangle {
    id: geometryControlRoot
    width: 320
    height: numberOfVoxels.height + geometry.height + okButton.height + 10
    color: "#ffffff"
    opacity: 0.9

    onVisibleChanged: {
        focus = visible
    }

    GroupBox {
        id: numberOfVoxels
        width: parent.width
        height: 56
        title: qsTr("Number of voxels")

        TextField {
            id: numVoxelsX
            x: 0
            y: -2
            width: 45
            height: 22
            placeholderText: qsTr("NX")
            onFocusChanged: if(focus) selectAll()
            text: marchingCubes.numVoxels.x
        }

        TextField {
            id: numVoxelsY
            x: 51
            y: -2
            width: 45
            height: 22
            placeholderText: qsTr("NY")
            onFocusChanged: if(focus) selectAll()
            text: marchingCubes.numVoxels.y
        }

        TextField {
            id: numVoxelsZ
            x: 102
            y: -2
            width: 45
            height: 22
            placeholderText: qsTr("NZ")
            onFocusChanged: if(focus) selectAll()
            text: marchingCubes.numVoxels.z
        }
    }

    GroupBox {
        id: geometry
        anchors.top: numberOfVoxels.bottom
        width: parent.width
        height: 93
        title: qsTr("Geometries")

        Button {
            id: sinus
            text: qsTr("Sinus")
            onClicked: {
                simulator.geometry = MySimulator.SINUS
                marchingCubes.threshold = 0
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
                geometryControlRoot.visible = false
            }
        }

        Button {
            id: sphere
            anchors.left: sinus.right
            text: qsTr("Sphere")
            onClicked: {
                simulator.geometry = MySimulator.SPHERE
                marchingCubes.threshold = 3
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
                geometryControlRoot.visible = false
            }
        }

        Button {
            id: cube
            anchors.left: sphere.right
            text: qsTr("Cube")
            onClicked: {
                simulator.geometry = MySimulator.CUBE
                marchingCubes.threshold = 3
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
                geometryControlRoot.visible = false
            }
        }

        Button {
            id: perlin
            anchors.top: sinus.bottom
            text: qsTr("Perlin")
            onClicked: {
                simulator.geometry = MySimulator.PERLIN
                marchingCubes.threshold = 0.6
                marchingCubes.numVoxels = Qt.vector3d(numVoxelsX.text, numVoxelsY.text, numVoxelsZ.text)
                geometryControlRoot.visible = false
            }
        }
    }

    Button {
        id: okButton
        x: parent.width*0.5 - width*0.5
        anchors.top: geometry.bottom
        anchors.topMargin: 5
        text: qsTr("OK")
        onClicked: geometryControlRoot.visible = false
    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Escape) {
            visible = false
        }
    }
}

