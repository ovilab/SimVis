import QtQuick 2.3
import QtQuick.Controls 1.2
import SimVis 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: console.log("Open action triggered");
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    Label {
        text: shaderGraph.shaderSource
        anchors.centerIn: parent
    }

    ShaderGraph {
        id: shaderGraph

        property real a: 10

//        ShaderNode {
////            inputs: [
////                "float a",
////                "float b"
////            ]
////            outputs: [
////                "float sum"
////            ]
////            shaderSource: "sum = a + b;"

//            //            outputs: [
//            //                "float sum"
//            //            ]
//            //            mappings: {
//            //                "a":
//            //            }
//        }

        ShaderNode {
            id: adderNode
            input: ShaderNodeInput {
                property real a
                property real b
            }
            output: ShaderNodeOutput {
                property real sum
            }
            shaderSource: "sum = a + b;"
            ShaderBinding {
                source: shaderGraph
                sourceProperty: "a"
                targetProperty: "a"
            }
        }
        ShaderNode {
            id: adderNode2
            input: ShaderNodeInput {
                property real a
                property real b
            }
            output: ShaderNodeOutput {
                property real sum
            }
            shaderSource: "sum = a + b;"
            ShaderBinding {
                source: adderNode
                sourceProperty: "a"
                targetProperty: "a"
            }
        }


    }
}
