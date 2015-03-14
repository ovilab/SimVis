import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import MySimulator 1.0
ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    MySimulator {

    }

    Visualizer {
        width: 100
        height: 100
        Billboards2D {
            id: test
        }
        Component.onCompleted: {
            update()
        }
    }
}
