import SimVis 1.0
import Qt3D.Render 2.0

ShaderNode {
    property alias source: textureImage.source
    property var height: 0.0
    property var vector: ShaderBuilderBinding {
        property: "textureCoordinate"
    }
    property var normal: ShaderBuilderBinding {
        property: "normal"
    }
    property var position: ShaderBuilderBinding {
        property: "position"
    }
    property var distance: 0.1
    property var strength: 1.0

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

    name: "bumpimage"
    type: "vec3"
    result: {
        if(glslType(texture) === "sampler2D") {
            return "bumpImage($(height, float), $(vector, vec2), $(position, vec3), $(normal, vec3), " +
                    "$(distance, float), $(strength, float))"
        }
        return "$(texture, " + type + ")"
    }
    header: "
vec3 bumpImage(float height, vec2 coordinate, vec3 position, vec3 normal, float delta, float strength) {
    vec3 dPositiondx = dFdx(position);
    vec3 dPositiondy = dFdy(position);

    vec3 Rx = cross(dPositiondy, normal);
    vec3 Ry = cross(normal, dPositiondx);

    float determinant = dot(dPositiondx, Rx);
    float absdeterminant = abs(determinant);

    float dBs = dFdx(height);
    float dBt = dFdy(height);

    vec3 surfaceGradient = sign(determinant) * (dBs * Rx + dBt * Ry);

    vec3 normalOut = normalize(absdeterminant * normal - delta * sign(determinant) * surfaceGradient);
    normalOut = normalize(strength * normalOut + (1.0 - strength) * normal);
    return normalOut;
}
"
}
