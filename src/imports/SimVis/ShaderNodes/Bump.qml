import SimVis 1.0
import QtQuick 2.0 as QQ2

ShaderNode {
    type: "vec3"
    result: "vec3(0.0, 0.0, 0.0)"
    QQ2.Repeater {
        model: 2
        Simplex {

        }
    }
}
