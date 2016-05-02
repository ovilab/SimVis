import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import NeuroML 1.0

Scene3D {
    focus: true
    aspects: ["render", "logic", "input"]
    Visualizer {
        id: visualizer

        NeuronReader {
            id: simulator
            source: "L5PC.cell.nml"
        }

        Cylinders {
            id: cylinders
            cylinderData: simulator.cylinderData
//            fragmentColor: StandardMaterial {
//                color: ImageTexture {
//                    source: "diffuse.webp"
//                    vector: Multiply {
//                        value1: 2.0
//                        value2: cylinders.fragmentBuilder.position
//                    }
//                }
//                normal: NormalMap {
//                    color: ImageTexture {
//                        source: "normal.webp"
//                        vector: Multiply {
//                            value1: 2.0
//                            value2: cylinders.fragmentBuilder.position
//                        }
//                    }
//                }

//                specularIntensity: 10.0
//                hardness: 0.2
//                lights: ShaderGroup {
//                    Nodes.Light {
//                        position: Qt.vector3d(20.0, 0.0, -10.0)
//                        attenuation: 0.1
//                    }
//                    Nodes.Light {
//                        position: Qt.vector3d(-20.0, 0.0, 10.0)
//                        attenuation: 0.1
//                    }
//                    Nodes.Light {
//                        position: Qt.vector3d(-50.0, 10.0, 50.0)
//                        attenuation: 0.1
//                    }
//                    Nodes.Light {
//                        position: Qt.vector3d(50.0, -20.0, -50.0)
//                        attenuation: 0.1
//                    }
//                }
//            }
        }

        InstancedCylinders {}

        CylinderMesh {
            id: mesh
            radius: 1
            length: 3
            rings: 100
            slices: 20
        }

        Transform {
            id: transform
            scale: 1.5
            rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 45)
        }

        PhongMaterial {
            id: material
            diffuse: "green"
        }

        Entity {
            id: mainEntity
            objectName: "mainEntity"
            components: [ mesh, material, transform ]
        }
    }
}
