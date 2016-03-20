import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import MySimulator 1.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0
import QtQuick.Scene3D 2.0
import QtMultimedia 5.5

Item {
    id: applicationRoot
//    title: qsTr("Spheres in an harmonic oscillator - SimVis example")
    width: 1650
    height: 900
    visible: true

    FunctionSurfaceScene {
        id: scene
        anchors.fill: parent
        focus: true
    }

    Slider {
        id: slider
        minimumValue: 0.0
        maximumValue: 1.0
    }
    Text {
//        text: scene.rotation
    }
    MouseArea {
        anchors.fill: parent
        propagateComposedEvents: true
        hoverEnabled: true
        onMouseXChanged: {
            scene.chest.translation.x = mouse.x / 1000
            mouse.accepted = false
        }
        onMouseYChanged: {
            scene.chest.translation.y = mouse.y / 1000
            mouse.accepted = false
        }
    }
}



//import QtQuick 2.2
//import QtQuick.Controls 1.1
//import MySimulator 1.0
//import SimVis 1.0
//import SimVis.ShaderNodes 1.0

//ApplicationWindow {
//    id: applicationRoot
//    visible: true
//    width: 1600
//    height: 900
//    title: qsTr("Function surface demo")

//    MySimulator {
//        id: simulator
//        marchingCubes: MarchingCubes {
//            threshold: thresholdSlider.value
//            resolution: Qt.vector3d(64,64,64)
//            minValues: Qt.vector3d(-20, -20, -20)
//            maxValues: Qt.vector3d(20, 20, 20)
//        }
//    }

//    Visualizer {
//        id: visualizer
//        anchors.fill: parent
//        simulator: simulator
//        camera: camera
//        backgroundColor: "gray"
//        navigator: navigator
//        TrackballNavigator {
//            id: navigator
//            anchors.fill: parent
//            camera: camera
//        }

//        Camera {
//            id: camera
//            farPlane: 1000000
//            nearPlane: 0.1
//        }

//        FunctionSurface {
//            id: surface
//            threshold: thresholdSlider.value
//            scale: scaleSlider.value
//            definition: Function {
//                input: surface.geometryShader.position
//                // result: "sin(2.0 * $input.x - $input.y*$input.z) + cos(2.0 * $input.y) + sin(2.0 * $input.z)*cos(2.0*$input.y) + cos(cp_time) + sin(cos(cp_time)*tan(cp_time));"
//                // result: "cos($input.x)*cos($input.x) + cos($input.y)*cos($input.y) + cos($input.z)*cos($input.z);"
//                result: "$input.x*$input.x + $input.y*$input.y + $input.z*$input.z;"
//            }
////            definition: Noise {
////                position: surface.geometryShader.position
////            }

////            definition: Mix {
////                value1: Sine {
////                    value: surface.geometryShader.position
////                }
////                value2: Sine {
////                    value: 1.0
////                }
////                mix: 0.5
////            }
//            fragmentColor: StandardMaterial {
//                normal: surface.fragmentShader.normal
//                position: surface.fragmentShader.position
//                color: "purple"
//                lights: ShaderGroup {
//                    Light {
//                        position: Qt.vector3d(-50, -50, -50)
//                        strength: 0.6
//                    }
//                    Light {
//                        position: camera.position
//                        strength: 0.6
//                    }
//                    Light {
//                        position: Qt.vector3d(50, 0, 50)
//                        strength: 0.6
//                    }
//                }
//            }
//        }
//    }

//    Column {
//        Row {
//            Label {
//                text: "Threshold: "
//            }
//            Slider {
//                id: thresholdSlider
//                minimumValue: -5
//                maximumValue: 5
//                value: 1.0
//            }
//        }
//        Row {
//            Label {
//                text: "Scale: "
//            }

//            Slider {
//                id: scaleSlider
//                minimumValue: 0
//                maximumValue: 1
//                value: 0.1
//            }

//            TextField {
//                id: scaleText
//                text: parseFloat(scaleSlider.value).toFixed(2)
//            }

//            Button {
//                text: "Set"
//                onClicked: {
//                    var res = parseFloat(resolution.text)
//                    scaleSlider.value = res
//                }
//            }
//        }
//        Row {
//            Label {
//                text: "Resolution: "
//            }

//            TextField {
//                id: resolution
//                text: "32"
//            }

//            Button {
//                text: "Set"
//                onClicked: {
//                    var res = parseInt(resolution.text)
//                    surface.resolution = res
//                }
//            }
//        }
//    }
//}
