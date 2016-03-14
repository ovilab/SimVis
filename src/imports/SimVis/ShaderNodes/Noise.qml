import SimVis 1.0

ShaderNode {
    property var position: Qt.vector3d(0.0, 0.0, 0.0)
    property var scale: 1.0

    type: "float"
    name: "noise"
    result: "noise($(position, vec3), $(scale, float));"
    header: "
        float hash( float n ) { return fract(sin(n)*753.5453123); }
        float noise( vec3 x, float scale)
        {
            vec3 p = floor(x*scale);
            vec3 f = fract(x*scale);
            f = f*f*(3.0-2.0*f);

            float n = p.x + p.y*157.0 + 113.0*p.z;
            return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                           mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
                       mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                           mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
        }
    "
}
