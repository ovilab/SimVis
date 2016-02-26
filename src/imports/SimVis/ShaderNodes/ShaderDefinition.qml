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
        var shader = ""
        shader += "\n// ------  begin generated outputs ------\n\n"
        for(var i in outputs) {
            shader += "out " + outputs[i].type + " " + outputs[i].name + ";\n"
        }
        shader += "\n// ------  end generated outputs   ------\n\n"
        shader += "\n// ------  begin generated header  ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            if(value) {
                shader += value.generateHeader() + "\n"
            }
        }
        shader += "\n// ------   end generated header   ------\n\n"
        var body = ""
        body += "\n// ------   begin generated body   ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            if(value) {
                body += value.generateBody();
            }
        }
        body += "\n// ------    end generated body    ------\n\n"
        body += "\n// ------ begin output assignments ------\n\n"
        for(var i in outputs) {
            var value = outputs[i].value
            if(value) {
                body += outputs[i].name + " = " + convertGlslType(value, outputs[i].type) + ";\n";
            }
        }
        body += "\n// ------  end output assignments  ------\n\n"

        var indent = source.match(/^( *)\$generate_shader\(\);?/m)
        if(indent && indent.length > 1) {
            body = body.replace(/\n/g, "\n" + indent[1])
        }
        shader += source.replace(/\$generate_shader\(\);?/, body)

        return shader
    }

    function convertGlslType(value, targetType) {
        if(value.type === targetType) {
            return value.name
        }
        if(value.type === "vec3" && targetType === "vec4") {
            return "vec4(" + value.name + ", 1.0)"
        }
        console.warn("WARNING: could not convert " + value.type + " to " + targetType)
        return value.name
    }
}
