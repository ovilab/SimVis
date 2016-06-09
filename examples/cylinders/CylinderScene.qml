import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import CylinderSimulator 1.0

Scene3D {

    function rotate(angle) {
        cylinders.transform.rotation = dummy.fromEulerAngles(angle, angle * 2, angle * 3)
//        cylinders.transform.translation = Qt.vector3d(angle, angle, -angle)
//        cylinders.transform.scale3D = Qt.vector3d(angle, 2*angle, 3*angle)
        console.log(cylinders.transform.rotation)
    }

    aspects: ["render", "logic", "input"]
    Visualizer {
        id: visualizer

        CylinderSimulator {
            id: simulator
        }

        OrbitController {
            camera: visualizer.camera
        }

        Cylinders {
            id: cylinders
            cylinderData: simulator.cylinderData
            fragmentColor: StandardMaterial {
                color: ImageTexture {
                    source: "diffuse.webp"
                }
                normal: NormalMap {
                    color: ImageTexture {
                        source: "normal.webp"
                    }
                }
                ambientIntensity: 10.0
                hardness: 2.0
            }
//            transform.scale3D: Qt.vector3d(1, 2, 3)
            Transform {
                id: dummy
            }
        }
    }
}
