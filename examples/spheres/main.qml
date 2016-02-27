import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

ApplicationWindow {
    id: applicationRoot
    title: qsTr("Spheres in an harmonic oscillator - SimVis example")
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
        backgroundColor: "#000"
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

//            fragColor: Mix {
//                value1: spheres.shader.normal
//                value2: spheres.shader.normal
//                mix: UniformValue {
//                    value: Qt.vector3d(1.0, 0.0, 1.0)
//                }
//            }

            fragColor: Diffuse {
                position: spheres.shader.position
                color: Mix {
                    value1: spheres.shader.normal
                    value2: UniformValue {
                        type: "vec3"
                        value: Qt.vector3d(0.1, 0.7, 0.9)
                    }
                    mix: UniformValue {
                        value: mixSlider.value
                    }
                }
                normal: Simplex {
                    normal: spheres.shader.normal
                    position: spheres.shader.normal
                }
            }

            Light {
                id: light
                ambientColor: "white"
                specularColor: "white"
                diffuseColor: "white"
                ambientIntensity: 0.05
                diffuseIntensity: 1.0
                specularIntensity: 0.1
                shininess: 2.0
                attenuation: 0.01
                position: Qt.vector3d(0.5, 0.5, 0.5)
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

            Row {
                Text {
                    text: "Mix 1:"
                }
                Slider {
                    id: mixSlider
                    minimumValue: 0.0
                    maximumValue: 1.0
                    value: 0.5
                }
            }

            //            Row {
            //                Text {
            //                    text: "Mix 2:"
            //                }
            //                Slider {
            //                    id: mixSlider2
            //                    minimumValue: 0.0
            //                    maximumValue: 1.0
            //                    value: 0.5
            //                }
            //            }

        }
    }

    Camera {
        id: camera
        nearPlane: 0.1
    }
}
