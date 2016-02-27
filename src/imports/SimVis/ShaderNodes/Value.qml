import QtQuick 2.0

QtObject {
    id: valueRoot

    default property var children

    property list<QtObject> depends
    property string name: randomName()
    property string type: ""
    property string initialization: ""
    property string result: ""
    property string header: ""

    property bool isUniform: false

    property bool _hasGeneratedBody: false
    property bool _hasGeneratedHeader: false
    readonly property string identifier: name + "_" + randomName()

    function convert(targetType) {
        if(type === targetType) {
            return identifier
        }
        var scalar = {
            "bool": "bool(" + identifier + ")",
            "int": "int(" + identifier + ")",
            "uint": "uint(" + identifier + ")",
            "float": "float(" + identifier + ")",
            "double": "double(" + identifier + ")",
            "vec3": "vec2(" + identifier + ", 0.0)",
            "vec3": "vec3(" + identifier + ", 0.0, 0.0)",
            "vec4": "vec4(" + identifier + ", 0.0, 0.0, 1.0)"
        }
        var conversions = {
            // first type is from, second is to, third is result
            "bool": scalar,
            "int": scalar,
            "uint": scalar,
            "float": scalar,
            "double": scalar,
            "vec2": {
                "float": "0.5 * (" + identifier + ".x + " + identifier + ".y)",
                "vec3": "vec3(" + identifier + ", 0.0)",
                "vec4": "vec4(" + identifier + ", 0.0, 1.0)"
            },
            "vec3": {
                "float": "1.0 / 3.0 * (" + identifier + ".x + " + identifier + ".y + " + identifier + ".z)",
                "vec2": identifier + ".xy",
                "vec4": "vec4(" + identifier + ", 1.0)"
            },
            "vec4": {
                "float": "0.25 * (" + identifier + ".x + " + identifier + ".y + " + identifier + ".z + " + identifier + ".w)",
                "vec2": identifier + ".xy",
                "vec3": identifier + ".xyz"
            }
        }
        if(conversions[type] && conversions[type][targetType]) {
            return "(" + conversions[type][targetType] + ")"
        }
        console.warn("WARNING: No known conversion from " + type + " to " + targetType)
        return identifier
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
        return text.replace(/\$/, identifier + "_")
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
                bodyResult += type + " " + identifier + ";\n";
            }
            if(initialization) {
                bodyResult += replaceWithName(initialization) + "\n";
            }
            if(result) {
                bodyResult += identifier + " = " + replaceWithName(result) + ";\n";
            }
            bodyResult += "\n";
            _hasGeneratedBody = true;
        }
        return bodyResult
    }
}
