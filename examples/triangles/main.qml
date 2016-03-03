import QtQuick 2.5
import QtQuick.Window 2.2
import MySimulator 1.0
import SimVis 1.0

Window {
    visible: true
    width: 1024
    height: 720

    MySimulator {
        id: simulator
    }

    Visualizer {
        id: visualizer
        anchors.fill: parent
        simulator: simulator
        camera: camera
        backgroundColor: "black"

        SkyBox {
            id: skybox
            camera: camera
            texture: ":/cubemap.jpg"
        }

        TriangleCollection {
            id: triangles

            Light {
                id: light
                ambientColor: "blue"
                specularColor: "white"
                diffuseColor: "green"
                ambientIntensity: 0.025
                diffuseIntensity: 0.5
                specularIntensity: 0.01
                shininess: 100.0
                attenuation: 0.01
                position: camera.position
            }

            SkyBoxReflection {
                id: reflection
                skybox: skybox
                reflectivity: 0.2
            }

            SimplexBump {
                id: simplexBump
                enabled: true
                intensity: 0.1
                scale: 1.0
            }

//            SimplexTexture {
//                id: simplexTexture
//                scale: 1.0
//                enabled: true
//                timeDependent: true
//            }
        }

        TrackballNavigator {
            id: navigator
            anchors.fill: parent
            camera: camera
        }
    }

    Camera {
        id: camera
        farPlane: 1000000
        nearPlane: 0.1
    }
}

