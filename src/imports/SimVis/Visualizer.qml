import SimVis 1.0

import Qt3D.Core 2.0
import Qt3D.Render 2.0

Entity {
    id: visualizerRoot
    property alias clearColor: forwardRenderer.clearColor
    property Camera camera: Camera {
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 50
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 10000.0
        position: Qt.vector3d(0.0, 0.0, 5.0)
        upVector: Qt.vector3d(0.0, 1.0, 0.0)
        viewCenter: Qt.vector3d(0.0, 0.0, 0.0)
    }

    Configuration  {
        controlledCamera: visualizerRoot.camera
    }

    components: [
        FrameGraph {
            id: frameGraph
            activeFrameGraph: NonCullingForwardRenderer {
                id: forwardRenderer
                camera: visualizerRoot.camera
                clearColor: "#012"
            }
        }
    ]
}
