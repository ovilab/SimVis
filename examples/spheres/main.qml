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

        Spheres {
            id: spheres
            visible: true
            scale: scaleSlider.value

            fragColor: Diffuse {
                position: spheres.shader.position
                color: Mix {
                    id: mixer
                    value1: spheres.shader.normal
                    value2: Qt.vector3d(0.2, 0.7, 0.9)
                    mix: split.a
                }
                normal: Simplex {
                    normal: spheres.shader.normal
                    position: normal
                    scale: split.b
                    intensity: mixSlider4.value
                }
            }

            Split {
                id: split
                input: Mix {
                    value1: 0.3
                    value2: 10.0
                    mix: mixSlider.value
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
        width: statusColumn.width
        height: statusColumn.height

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
                Text {
                    text: mixSlider.value.toFixed(2)
                }
            }

            Row {
                Text {
                    text: "Mix 2:"
                }
                Slider {
                    id: mixSlider2
                    minimumValue: 0.0
                    maximumValue: 1.0
                    value: 0.5
                }
                Text {
                    text: mixSlider.value.toFixed(2)
                }
            }

            Row {
                Text {
                    text: "Scale:"
                }
                Slider {
                    id: mixSlider3
                    minimumValue: 0.0
                    maximumValue: 10.0
                    value: 0.5
                }
                Text {
                    text: mixSlider3.value.toFixed(2)
                }
            }

            Row {
                Text {
                    text: "Intensity:"
                }
                Slider {
                    id: mixSlider4
                    minimumValue: 0.0
                    maximumValue: 10.0
                    value: 0.2
                }
                Text {
                    text: mixSlider4.value.toFixed(2)
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
