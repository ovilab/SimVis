import SimVis 1.0

ShaderNode {
    property ShaderGroup inputs

    type: "float"
    source: {
        var output = "";
        output += "float $sumresult = 0.0;\n"
        for(var i in inputs.nodes) {
            output += "$sumresult += $(inputs[" + i + "], float);\n";
        }
        return output
    }
    result: "$sumresult"
}
