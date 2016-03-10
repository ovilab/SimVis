import QtQuick 2.2
import QtQuick.Controls 1.1
import MySimulator 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

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

//        SkyBox {
//            id: skybox
//            camera: camera
//            texture: ":/cubemap.jpg"
//        }

        MarchingCubes {
            id: marchingCubes
            numVoxels: Qt.vector3d(100, 100, 100)
            min: Qt.vector3d(-Math.PI, -Math.PI, -Math.PI)
            max: Qt.vector3d(Math.PI, Math.PI, Math.PI)
            threshold: 0.0
            mode: MarchingCubes.FRONT_AND_BACK
            color: "#fd20c1"
            fragmentColor: Mix {
                value1: Qt.vector3d(1.0, 0.0, 0.3)
                value2: Qt.vector3d(1.0, 0.3, 0.3)
                mix: 0.5
            }

//            SimplexBump {
//                id: simplexBump
//                enabled: false
//                intensity: 0.1
//                scale: 1.0
//            }

//            Light {
//                id: light
//                ambient: true
//                diffuse: true
//                specular: true
//                ambientColor: color
//                specularColor: "white"
//                diffuseColor: color
//                ambientIntensity: 0.1
//                diffuseIntensity: 1.0
//                specularIntensity: 1.0
//                shininess: 30.0
//                attenuation: 0.01
//                position: camera.position
//            }

//            SkyBoxReflection {
//                id: reflection
//                skybox: skybox
//                reflectivity: 0.2
//            }

//            SimplexTexture {
//                id: simplexTexture
//                scale: 1.0
//                enabled: false
//                timeDependent: true
//            }
        }
    }

    Item {
        x: 0.5*(parent.width - width)
        height: 50
        width: lightButton.width + geometryButton.width + renderButton.width + 5

        Button {
            anchors.left: parent.left
            anchors.leftMargin: 3
            id: lightButton
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

        Label {
            anchors.top: lightButton.bottom
            x: 0.5*(parent.width - width)
            // anchors.left: parent.left
            // anchors.leftMargin: 3
            anchors.topMargin: 5
            id: triangleCount
            text: {
                if(marchingCubes.mode == MarchingCubes.LINES) {
                    return marchingCubes.numberOfLines+" lines with "+ visualizer.fps.toFixed(1) +" fps."
                } else {
                    return marchingCubes.numberOfTriangles+" triangles with "+ visualizer.fps.toFixed(1) +" fps."
                }
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
