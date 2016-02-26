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
