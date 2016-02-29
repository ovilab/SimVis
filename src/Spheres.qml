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
        inputs: [
            ShaderOutput {
                type: "vec3"
                name: "in_position"
            },
            ShaderOutput {
                type: "vec3"
                name: "in_color"
            },
            ShaderOutput {
                type: "float"
                name: "in_scale"
            },
            ShaderOutput {
                type: "float"
                name: "in_sphereId"
            }
        ]
        outputs: _geometryShader.inputs
    }

    ShaderBuilder {
        id: vertexShaderNoGeometry
        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node.vsh"
        shaderType: ShaderBuilder.Vertex
        inputs: [
            ShaderOutput {
                type: "vec3"
                name: "in_position"
            },
            ShaderOutput {
                type: "vec3"
                name: "in_color"
            },
            ShaderOutput {
                type: "vec2"
                name: "in_texCoord"
            },
            ShaderOutput {
                type: "float"
                name: "in_sphereId"
            },
            ShaderOutput {
                type: "float"
                name: "in_scale"
            },
            ShaderOutput {
                type: "float"
                name: "in_vertexId"
            }
        ]
        outputs: fragmentShader.inputs
    }

    geometryShader: spheresRoot.isGeometryShaderSupported ? _geometryShader : null

    ShaderBuilder {
        id: _geometryShader
        sourceFile: "qrc:/org.compphys.SimVis/renderables/spheres/spheres_node.gsh"
        shaderType: ShaderBuilder.Geometry
        inputs: [
            ShaderOutput {
                type: "vec3"
                name: "vs_position"
            },
            ShaderOutput {
                type: "vec3"
                name: "vs_color"
            },
            ShaderOutput {
                type: "float"
                name: "vs_scale"
            },
            ShaderOutput {
                type: "float"
                name: "vs_sphereId"
            }
        ]
        outputs: fragmentShader.inputs
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

        inputs: [
            ShaderOutput {
                type: "vec3"
                name: "color"
            },
            ShaderOutput {
                type: "vec2"
                name: "texCoord"
            },
            ShaderOutput {
                type: "vec3"
                name: "position"
            },
            ShaderOutput {
                type: "float"
                name: "sphereId"
            }
        ]

        outputs: [
            ShaderOutput {
                type: "vec4"
                name: "gl_FragColor"
                value: _shader.fragColor
            }
        ]

        // TODO consider adding support for chaining shaders
        // TODO add functionality to choose input names or shader file based on GLSL version
    }
}

