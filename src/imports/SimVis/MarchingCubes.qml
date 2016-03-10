import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractMarchingCubes {
    id: marhcingCubesRoot
//    property alias fragmentColor: _fragmentShader.fragColor

//    vertexShader: ShaderBuilder {
//        id: _vertexShader
//        sourceFile: "qrc:/org.compphys.SimVis/renderables/marchingcubes/marchingcubes.vsh"
//        shaderType: ShaderBuilder.Vertex
//    }

//    fragmentShader: ShaderBuilder {
//        id: _fragmentShader

//        // TODO add readonly or some other way to show that these are only for others to read
//        shaderType: ShaderBuilder.Fragment

//        // inputs
//        property ShaderNode position: ShaderNode {
//            type: "vec3"
//            name: "position"
//            result: "position"
//        }
//        property ShaderNode normal: ShaderNode {
//            type: "vec3"
//            name: "normal"
//            result: "normal"
//        }
//        property ShaderNode color: ShaderNode {
//            type: "vec3"
//            name: "color"
//            result: "color"
//        }

//        // outputs
//        // TODO fix this so that it can be var
//        property ShaderNode fragColor: StandardMaterial {
//            position: _fragmentShader.position
//            color: _fragmentShader.normal
//            normal: _fragmentShader.normal
//        }

//        sourceFile: "qrc:/org.compphys.SimVis/renderables/marchingcubes/marchingcubes.fsh"

//        outputs: [
//            ShaderOutput {
//                type: "vec4"
//                name: "cp_FragColor"
//                value: _fragmentShader.fragColor
//            }
//        ]

//        // TODO consider adding support for chaining shaders
//        // TODO add functionality to choose input names or shader file based on GLSL version
//    }
}

