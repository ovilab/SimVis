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
        backgroundColor: "#111"
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
                color: Qt.vector3d(1.0, 1.0, 1.0)
                normal: spheres.shader.normal
                lights: ShaderGroup {
                    id: group
                    property real scale: 100.0
                    Light {
                        diffuseColor: Qt.vector3d(Math.random(), Math.random(), Math.random())
                        position: Qt.vector3d((2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale)
                    }
                    Light {
                        diffuseColor: Qt.vector3d(Math.random(), Math.random(), Math.random())
                        position: Qt.vector3d((2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale)
                    }
                    Light {
                        diffuseColor: Qt.vector3d(Math.random(), Math.random(), Math.random())
                        position: Qt.vector3d((2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale, (2.0*Math.random() - 1.0)*group.scale)
                    }
                }
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
