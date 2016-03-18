import SimVis 1.0
import QtQuick 2.0 as QQ2

ShaderNode {
    property alias height: variation.target
    property var variation: VariationGroup {
        id: variation
        property: "offset"
        VariationNode {
            value: Qt.vector3d(0.0, 0.0, 0.0)
        }
        VariationNode {
            value: Qt.vector3d(0.05, 0.05, 0.05)
        }
        VariationNode {
            value: Qt.vector3d(-0.05, -0.05, -0.05)
        }
    }
    type: glslType(variation.target)
    name: "bump"
    result: "1.0 / 3.0 * ($variation[0] + $variation[1] + $variation[2])"
}
