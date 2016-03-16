import SimVis 1.0

ShaderNode {
    property var texture
    property var texCoord

    name: "imagetexture"
    type: "vec4"
    result: {
        if(glslType(texture) === "sampler2D") {
            return "texture($texture, $(texCoord, vec2))"
        }
        return "$(texture, " + type + ")"
    }
}
