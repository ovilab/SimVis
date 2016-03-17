import Qt3D.Core 2.0
import Qt3D.Render 2.0

import QtQuick 2.0 as QQ2

import QtQuick.Scene3D 2.0

import MySimulator 1.0

Entity {
    property alias translation: transform.translation
    components: [ transform, mesh, testMaterial ]
    
    Transform {
        id: transform
        scale: 0.03
        rotation: fromAxisAndAngle(Qt.vector3d(1, 0, 0), 0.0)
        translation: Qt.vector3d(0, 0, -5)
    }
    
    Mesh {
        id: mesh
        source: "Chest.obj"
    }
    
//    Material {
//        id: testMaterial
//        effect: Effect {
//            techniques: Technique {
//                renderPasses: RenderPass {
//                    shaderProgram: ShaderProgram {
//                        vertexShaderCode: loadSource("qrc:/regular.vert")
//                        fragmentShaderCode: loadSource("qrc:/regular.frag")
//                    }
//                }
//            }
//        }
//    }
    DiffuseMapMaterial {
        id: testMaterial
        diffuse: "diffuse.webp"
    }
}
