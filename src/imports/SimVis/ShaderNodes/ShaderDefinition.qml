import QtQuick 2.0

QtObject {
    default property var children

    property list<OutputValue> outputs
    property string source

    function findUniforms() {
        var uniforms = []
        for(var i in outputs) {
            var value = outputs[i].value
            if(value) {
                uniforms = uniforms.concat(outputs[i].value.findUniforms())
            }
        }
        return uniforms
    }

    readonly property string finalShader: {
        // Verify all that all outputs have values
        for(var i in outputs) {
            var value = outputs[i].value
            if(!value) {
                throw "ShaderDefintion output " + outputs[i].name + " has no value: " + value
            }
        }

        var contents = ""
        contents += "\n// ------  begin generated header  ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            contents += value.generateHeader() + "\n"
        }
        contents += "\n// ------   end generated header   ------\n\n"
        contents += "\n// ------  begin generated outputs ------\n\n"
        for(var i in outputs) {
            contents += "out " + outputs[i].type + " " + outputs[i].name + ";\n"
        }
        contents += "\n// ------  end generated outputs   ------\n\n"

        var setup = ""
        setup += "\n// ------   begin generated body   ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            setup += value.generateBody();
        }
        setup += "\n// ------    end generated body    ------\n\n"
        setup += "\n// ------ begin output assignments ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            setup += outputs[i].name + " = " + value.convert(outputs[i].type) + ";\n";
        }
        setup += "\n// ------  end output assignments  ------\n\n"
        var matchString = "\\$setupShaderNodes\\(\\);?"

        var indent = source.match(new RegExp("^( *)" + matchString, "m"))
        if(indent && indent.length > 1) {
            setup = setup.replace(/\n/g, "\n" + indent[1])
        }
        contents += source.replace(new RegExp(matchString), setup)

        return contents
    }
}
