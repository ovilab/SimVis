import QtQuick 2.0

QtObject {
    id: valueRoot

    default property var children

    property list<QtObject> depends
    property string identifier: randomName()
    property string type: ""
    property string initialization: ""
    property string result: ""
    property string header: ""

    property bool isUniform: false

    property bool _hasGeneratedBody: false
    property bool _hasGeneratedHeader: false
    readonly property string name: identifier + "_" + randomName()

    function convert(targetType) {
        if(type === targetType) {
            return name
        }
        var scalar = {
            "bool": "bool(" + name + ")",
            "int": "int(" + name + ")",
            "uint": "uint(" + name + ")",
            "float": "float(" + name + ")",
            "double": "double(" + name + ")",
            "vec3": "vec2(" + name + ", 0.0)",
            "vec3": "vec3(" + name + ", 0.0, 0.0)",
            "vec4": "vec4(" + name + ", 0.0, 0.0, 1.0)"
        }
        var conversions = {
            // first type is from, second is to, third is result
            "bool": scalar,
            "int": scalar,
            "uint": scalar,
            "float": scalar,
            "double": scalar,
            "vec2": {
                "float": "0.5 * (" + name + ".x + " + name + ".y)",
                "vec3": "vec3(" + name + ", 0.0)",
                "vec4": "vec4(" + name + ", 0.0, 1.0)"
            },
            "vec3": {
                "float": "1.0 / 3.0 * (" + name + ".x + " + name + ".y + " + name + ".z)",
                "vec2": name + ".xy",
                "vec4": "vec4(" + name + ", 1.0)"
            },
            "vec4": {
                "float": "0.25 * (" + name + ".x + " + name + ".y + " + name + ".z + " + name + ".w)",
                "vec2": name + ".xy",
                "vec3": name + ".xyz"
            }
        }
        if(conversions[type] && conversions[type][targetType]) {
            return "(" + conversions[type][targetType] + ")"
        }
        console.warn("WARNING: No known conversion from " + type + " to " + targetType)
        return name
    }

    function randomName()
    {
        var text = "";
        var possible = "abcdefghijklmnopqrstuvwxyz";

        for( var i=0; i < 4; i++ )
            text += possible.charAt(Math.floor(Math.random() * possible.length));

        return text;
    }

    function findUniforms() {
        var uniforms = []
        if(isUniform) {
            uniforms = [valueRoot]
        }
        for(var i in depends) {
            if(depends[i]) {
                uniforms = uniforms.concat(depends[i].findUniforms())
            }
        }
        return uniforms
    }

    function replaceWithName(text) {
        return text.replace(/\$/, name + "_")
    }

    function generateHeader() {
        var headerResult = ""
        if(!_hasGeneratedHeader) {
            for(var i in depends) {
                if(depends[i]) {
                    headerResult += depends[i].generateHeader();
                }
            }
            headerResult += replaceWithName(header)
            _hasGeneratedHeader = true
        }
        return headerResult
    }

    function generateBody() {
        if(isUniform) {
            return ""
        }
        if(type == "") {
            throw "Error! Type is missing from Value."
        }

        var bodyResult = "";
        if(!_hasGeneratedBody) {
            for(var i in depends) {
                if(depends[i]) {
                    bodyResult += depends[i].generateBody();
                }
            }
            if(type) {
                bodyResult += type + " " + name + ";\n";
            }
            if(initialization) {
                bodyResult += replaceWithName(initialization) + "\n";
            }
            if(result) {
                bodyResult += name + " = " + replaceWithName(result) + ";\n";
            }
            bodyResult += "\n";
            _hasGeneratedBody = true;
        }
        return bodyResult
    }
}
