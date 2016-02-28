import QtQuick 2.0
import SimVis 1.0

ShaderNode {
    property var input

    // TODO use readonly instead of OutputNode
    property var a: OutputNode {
        name: "split_a"
        type: "float"
    }
    property var b: OutputNode {
        name: "split_b"
        type: "float"
    }

    name: "split"
    type: a.type

    source: "
float tmpval = sqrt($(input, float));
$a = tmpval * 2.0;
$b = tmpval * 4.0;
"
    result: "$a"
}
