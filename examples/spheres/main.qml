import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0

ApplicationWindow {
    id: applicationRoot
    title: qsTr("Footballs in a harmonic oscillator - SimVis example")
    width: 1650
    height: 900
    visible: true

    MySimulator {
        id: simulator
        dt: dt.value
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
        camera: camera
        backgroundColor: "#fff"
        navigator: navigator

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }

//        SkyBox {
//            id: skybox
//            camera: camera
//            texture: ":/1024.png"
//        }

        Spheres {
            id: spheres
            visible: true
            scale: scaleSlider.value
            color: "#ff17e6"

            Light {
                id: light
                ambientColor: "yellow"
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.5
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(0.5, 0.5, 0.5)// Qt.vector3d(camera.position.x + 0.25, camera.position.y + 0.25, camera.position.z + 0.25)
            }

            Light {
                id: light2
                ambientColor: spheres.color
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.025
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: Qt.vector3d(Math.sin(0.3*6.28*visualizer.time), Math.cos(0.223*6.28*visualizer.time), 0)
            }

            Light {
                id: light3
                ambientColor: spheres.color
                specularColor: "white"
                diffuseColor: spheres.color
                ambientIntensity: 0.025
                diffuseIntensity: 0.5
                specularIntensity: 1.0
                shininess: 40.0
                attenuation: 0.01
                position: camera.position
            }

//            SkyBoxReflection {
//                id: reflection
//                skybox: skybox
//                reflectivity: 0.2
//            }

            SimplexBump {
                id: simplexBump
                enabled: true
                intensity: 0.1
                scale: 1.0
            }
        }
    }

    Rectangle {
        radius: 4
        color: Qt.rgba(1.0, 1.0, 1.0, 0.975)
        width: 350
        height: 100

        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.XAndYAxis
        }

        ColumnLayout {
            y: 10
            x: 10
            id: statusColumn
            Row {
                Text {
                    text: "Sphere scale: "
                }

                Slider {
                    id: scaleSlider
                    minimumValue: 0.1
                    maximumValue: 10.0
                    value: 1.0
                }
            }

            Row {
                Text {
                    text: "dt: "
                }

                Slider {
                    id: dt
                    minimumValue: 0.01
                    maximumValue: 1.0
                    value: 0.01
                }
            }

        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
    }
}
