import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import CylinderSimulator 1.0

Scene3D {
    aspects: ["render", "logic", "input"]
    Visualizer {
        CylinderSimulator {
            id: simulator
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
        }
    }
}
