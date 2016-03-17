import SimVis 1.0
import Qt3D.Render 2.0

ShaderNode {
    property alias source: textureImage.source
    property var vector: ShaderBuilderBinding {
        property: "textureCoordinate"
    }

    property var texture: Texture2D {
        id: diffuseTexture
        minificationFilter: Texture.LinearMipMapLinear
        magnificationFilter: Texture.Linear
        wrapMode {
            x: WrapMode.Repeat
            y: WrapMode.Repeat
        }
        generateMipMaps: true
        maximumAnisotropy: 16.0
        TextureImage {
            id: textureImage
        }
    }

    name: "imagetexture"
    type: "vec4"
    result: {
        if(glslType(texture) === "sampler2D") {
            return "texture($texture, $(vector, vec2))"
        }
        return "$(texture, " + type + ")"
    }
}
