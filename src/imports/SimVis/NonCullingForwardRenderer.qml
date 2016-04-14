import Qt3D.Core 2.0
import Qt3D.Render 2.0

TechniqueFilter {
    property alias camera: cameraSelector.camera
    property alias clearColor: viewport.clearColor
    property alias viewportRect: viewport.rect
    requires: [
        Annotation {
            name: "renderingStyle"
            value: "forward"
        }
    ]

    Viewport {
        id: viewport
        rect: Qt.rect(0.0, 0.0, 1.0, 1.0)
        clearColor: "blue"
        CameraSelector {
            id : cameraSelector
            FrustumCulling {
                enabled: false
                ClearBuffer {
                    buffers : ClearBuffer.ColorDepthBuffer
                }
            }
        }
    }
}
