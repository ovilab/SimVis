import QtQuick 2.0
import SimVis 1.0
import SimVis.ShaderNodes 1.0

AbstractSpheres {
    id: spheresRoot
    property alias fragColor: _shader.fragColor
    property alias shader: _shader

    vertexShader: spheresRoot.isGeometryShaderSupported ? vertexShaderGeometry : vertexShaderNoGeometry

    ShaderBuilder {
        id: vertexShaderGeometry
        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node_geom.vsh"
        shaderType: ShaderBuilder.Vertex
    }

    ShaderBuilder {
        id: vertexShaderNoGeometry
        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node.vsh"
        shaderType: ShaderBuilder.Vertex
    }

    geometryShader: spheresRoot.isGeometryShaderSupported ? _geometryShader : null

    ShaderBuilder {
        id: _geometryShader
        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node.gsh"
        shaderType: ShaderBuilder.Geometry
    }

    fragmentShader: ShaderBuilder {
        id: _shader

        // TODO add readonly or some other way to show that these are only for others to read
        shaderType: ShaderBuilder.Fragment
        property ShaderNode position: ShaderNode {
            type: "vec3"
            name: "position"
            result: "position";
        }
        property ShaderNode normal: ShaderNode {
            type: "vec3"
            name: "normal"
            result: "normal";
        }
        property ShaderNode texCoord: ShaderNode {
            type: "vec2"
            name: "texCoord"
            result: "texCoord";
        }
        property ShaderNode color: ShaderNode {
            type: "vec3"
            name: "color"
            result: "color";
        }
        property ShaderNode sphereId: ShaderNode {
            type: "float"
            name: "sphereId"
            result: "sphereId";
        }

        property ShaderNode fragColor: Diffuse {
            position: shader.position
            color: shader.normal
            normal: shader.normal
        }

        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node.fsh"

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "cp_FragColor"
                value: _shader.fragColor
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
    }
}

