import QtQuick 2.6

Item {
    id: applicationRoot
    width: 1650
    height: 900
    visible: true

    BondScene {
        id: scene
        anchors.fill: parent
        focus: true
    }

    Shortcut {
        sequence: StandardKey.Copy
        onActivated: {
            console.log("Woop")
            scene.doSomething()
        }
    }
}

