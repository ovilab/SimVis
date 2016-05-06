import SimVis 1.0
import SimVis.ShaderNodes 1.0
import SimVis.ShaderNodes 1.0 as Nodes

import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2
import QtQuick.Scene3D 2.0

import NeuroML 1.0

Scene3D {
    property alias transforma: cylinders.transform
    aspects: "input"
    Visualizer {
        id: visualizer

        NeuronReader {
            id: simulator
            source: "L5PC.cell.nml"
        }

        Cylinders {
            id: cylinders
            cylinderData: simulator.cylinderData
        }
    }
}
