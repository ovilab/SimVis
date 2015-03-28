import QtQuick 2.2
import QtQuick.Controls 1.1
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    id: applicationRoot
    visible: true
    width: 1600
    height: 900
    title: qsTr("Marching cubes demo")

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        width: parent.width
        height: parent.height
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
            numVoxels: Qt.vector3d(100, 100, 100)
            min: Qt.vector3d(-Math.PI, -Math.PI, -Math.PI)
            max: Qt.vector3d(Math.PI, Math.PI, Math.PI)
            threshold: 0.0
            color: "#fd20c1"

            SimplexBump {
                id: simplexBump
                enabled: true
                intensity: 0.1
                scale: 1.0
            }

            DefaultLight {
                id: light
                ambient: true
                diffuse: true
                specular: true
                ambientColor: "green"
                specularColor: "white"
                diffuseColor: "blue"
                ambientIntensity: 0.1
                diffuseIntensity: 1.0
                specularIntensity: 1.0
                shininess: 30.0
                attenuation: 0.01
                position: camera.position
            }

            SimplexTexture {
                id: simplexTexture
                scale: 1.0
                enabled: false
                timeDependent: true
            }
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

    Item {
        width: 314
        height: 30
        opacity: 0.9

        Label {
            id: fpslabel
            text: "Fps: "+visualizer.fps.toFixed(1)
        }

        Button {
            id: lightButton
            anchors.left: fpslabel.right
            text: "Light"
            onClicked: {
                lightControl.visible = !lightControl.visible
            }
        }

        Button {
            id: geometryButton
            anchors.left: lightButton.right
            text: "Geometry"
            onClicked: {
                geometryControl.visible = !geometryControl.visible
            }
        }

        Button {
            id: renderButton
            anchors.left: geometryButton.right
            x: 175
            y: 0
            text: "Rendering"
            onClicked: {
                renderControl.visible = !renderControl.visible
            }
        }
    }

    LightControl {
        id: lightControl
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }

    GeometryControl {
        id: geometryControl
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }

    RenderControl {
        id: renderControl
        x: applicationRoot.width*0.5 - width*0.5
        y: applicationRoot.height*0.5 - height*0.5
        visible: false
    }
}
