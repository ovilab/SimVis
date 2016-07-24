const highp vec3 hemisphereVectors[128] = vec3[](
    vec3(0.291823, -0.261368, 0.965258),
    vec3(0.141762, 0.430384, 0.453689),
    vec3(0.817925, -0.251641, 0.452071),
    vec3(0.673906, -0.366821, 0.826028),
    vec3(0.890310, 0.453257, 0.942426),
    vec3(0.848005, -0.971413, 0.970335),
    vec3(-0.063863, -0.924945, 0.786908),
    vec3(-0.856224, -0.905766, 0.255042),
    vec3(0.837345, 0.725322, 0.374084),
    vec3(0.669941, 0.211320, 0.951211),
    vec3(-0.349745, 0.587258, 0.962050),
    vec3(-0.880726, -0.325341, 0.057600),
    vec3(0.146718, 0.908252, 0.905898),
    vec3(-0.320288, -0.261599, 0.657269),
    vec3(-0.548746, 0.989946, 0.907098),
    vec3(-0.856362, 0.213130, 0.658737),
    vec3(0.916742, 0.129737, 0.571573),
    vec3(0.655225, 0.139553, 0.352204),
    vec3(0.907804, 0.751607, 0.790281),
    vec3(0.751766, 0.981416, 0.134088),
    vec3(0.754731, 0.679354, 0.528277),
    vec3(-0.820974, 0.210065, 0.315682),
    vec3(0.933938, 0.490839, 0.711182),
    vec3(-0.699529, 0.507976, 0.880498),
    vec3(0.192341, -0.856663, 0.472571),
    vec3(-0.501042, -0.025966, 0.808096),
    vec3(0.822799, 0.104739, 0.070744),
    vec3(-0.812778, 0.225078, 0.986103),
    vec3(-0.086299, -0.668644, 0.985701),
    vec3(-0.333265, 0.700878, 0.799574),
    vec3(0.977192, -0.859158, 0.680767),
    vec3(0.209023, 0.097291, 0.966009),
    vec3(-0.197741, 0.814476, 0.929901),
    vec3(0.166330, 0.183546, 0.036923),
    vec3(0.882129, -0.902988, 0.818159),
    vec3(-0.258843, -0.783844, 0.951980),
    vec3(0.170608, -0.671053, 0.954558),
    vec3(-0.594573, -0.882929, 0.091138),
    vec3(-0.222972, 0.146186, 0.704628),
    vec3(-0.427093, 0.635928, 0.032092),
    vec3(-0.543282, -0.700130, 0.418790),
    vec3(0.524718, -0.284391, 0.713861),
    vec3(-0.382671, 0.222000, 0.142820),
    vec3(-0.021497, -0.610787, 0.083420),
    vec3(0.653822, -0.472195, 0.674940),
    vec3(-0.863795, 0.773315, 0.574176),
    vec3(0.097115, -0.646275, 0.873305),
    vec3(-0.594131, 0.695114, 0.090471),
    vec3(-0.646090, 0.645057, 0.757265),
    vec3(0.841703, -0.592665, 0.919895),
    vec3(0.086148, 0.032010, 0.072357),
    vec3(-0.213324, -0.244860, 0.254417),
    vec3(0.321221, 0.634677, 0.021588),
    vec3(-0.623614, 0.219928, 0.532597),
    vec3(-0.322187, 0.908745, 0.086188),
    vec3(-0.941172, -0.927770, 0.899665),
    vec3(0.858054, 0.768403, 0.990692),
    vec3(0.269614, 0.666812, 0.301824),
    vec3(0.982114, -0.617694, 0.606873),
    vec3(0.975747, 0.737794, 0.521899),
    vec3(-0.185706, -0.376185, 0.671966),
    vec3(0.196891, 0.097160, 0.538632),
    vec3(-0.003103, 0.134938, 0.408128),
    vec3(-0.497600, -0.131968, 0.409205),
    vec3(0.752115, 0.676954, 0.533084),
    vec3(0.778955, -0.013351, 0.621798),
    vec3(0.153026, 0.756975, 0.219411),
    vec3(0.009836, 0.545065, 0.775045),
    vec3(-0.586592, -0.776039, 0.287456),
    vec3(0.820161, 0.148931, 0.199971),
    vec3(0.952935, -0.984474, 0.894588),
    vec3(-0.890902, -0.984137, 0.268739),
    vec3(-0.726596, -0.365957, 0.582675),
    vec3(0.213249, -0.234933, 0.094191),
    vec3(0.589786, -0.927961, 0.704685),
    vec3(0.698487, 0.764162, 0.628806),
    vec3(-0.183107, -0.534328, 0.645142),
    vec3(0.572642, -0.824396, 0.590824),
    vec3(-0.267634, 0.686799, 0.366826),
    vec3(0.026281, -0.130362, 0.865222),
    vec3(-0.147215, -0.488608, 0.212625),
    vec3(-0.968248, -0.784954, 0.353227),
    vec3(-0.695470, -0.049020, 0.811701),
    vec3(0.879485, 0.995423, 0.141147),
    vec3(-0.609527, -0.415275, 0.720676),
    vec3(0.830998, 0.983542, 0.149835),
    vec3(0.259893, 0.871644, 0.780016),
    vec3(0.206704, -0.195293, 0.873701),
    vec3(-0.112471, 0.593813, 0.105511),
    vec3(-0.897100, -0.112124, 0.707912),
    vec3(0.535750, -0.627907, 0.299744),
    vec3(-0.717743, 0.758763, 0.579340),
    vec3(-0.901928, -0.462221, 0.751826),
    vec3(-0.801842, 0.258132, 0.113981),
    vec3(0.559160, 0.906951, 0.277619),
    vec3(-0.152657, 0.278342, 0.951740),
    vec3(-0.485233, -0.809967, 0.667643),
    vec3(-0.378391, 0.869146, 0.712426),
    vec3(-0.828886, -0.558130, 0.308863),
    vec3(0.633849, 0.016077, 0.119964),
    vec3(-0.219406, 0.374969, 0.936306),
    vec3(-0.923550, -0.119799, 0.046222),
    vec3(0.664117, 0.456696, 0.488519),
    vec3(-0.774331, 0.089627, 0.104948),
    vec3(-0.601550, -0.136176, 0.559716),
    vec3(-0.013103, -0.731433, 0.261241),
    vec3(0.838387, 0.240558, 0.995995),
    vec3(0.912588, 0.586466, 0.450290),
    vec3(0.112498, 0.090421, 0.466215),
    vec3(-0.529916, 0.062024, 0.045360),
    vec3(0.186889, 0.096795, 0.891482),
    vec3(-0.817214, -0.248727, 0.116260),
    vec3(0.661094, -0.104778, 0.671078),
    vec3(0.788862, -0.075574, 0.971641),
    vec3(-0.026305, -0.553343, 0.179994),
    vec3(-0.966658, 0.123398, 0.347291),
    vec3(-0.492391, -0.359681, 0.220991),
    vec3(0.995872, 0.905503, 0.917241),
    vec3(0.653046, -0.453422, 0.258743),
    vec3(0.781141, 0.053895, 0.398807),
    vec3(-0.907791, 0.900663, 0.003947),
    vec3(-0.178144, 0.072820, 0.549748),
    vec3(0.999736, -0.557339, 0.400953),
    vec3(0.492704, -0.130530, 0.002331),
    vec3(-0.298067, 0.041316, 0.687544),
    vec3(0.692447, 0.300721, 0.760804),
    vec3(0.386849, -0.067411, 0.620821),
    vec3(0.740663, -0.398783, 0.976784)
);


const highp vec3 sphereVectors[64] = vec3[](
    vec3(-0.682964, 0.778350, 0.542999),
    vec3(-0.708018, -0.366419, 0.593008),
    vec3(0.413995, 0.701051, 0.573785),
    vec3(0.019927, 0.895561, 0.111871),
    vec3(-0.619979, 0.604795, 0.920862),
    vec3(0.509113, -0.111283, 0.493613),
    vec3(0.756772, -0.523635, -0.223608),
    vec3(-0.420285, -0.804175, 0.952251),
    vec3(-0.735425, -0.204499, -0.201792),
    vec3(-0.825349, 0.420710, -0.237907),
    vec3(0.469124, 0.088359, -0.227493),
    vec3(-0.285993, 0.057321, 0.516281),
    vec3(-0.892817, -0.845720, -0.391775),
    vec3(0.967891, -0.634729, 0.521707),
    vec3(0.224459, -0.626506, 0.864145),
    vec3(-0.495659, -0.394431, -0.084059),
    vec3(0.949388, 0.136395, -0.438426),
    vec3(0.159124, -0.166975, -0.483519),
    vec3(0.629778, -0.904960, 0.344647),
    vec3(-0.070793, 0.487320, 0.719561),
    vec3(-0.772851, -0.178033, 0.508828),
    vec3(-0.071365, -0.707269, -0.019255),
    vec3(-0.771601, -0.642331, -0.283463),
    vec3(0.982604, 0.022148, 0.342875),
    vec3(0.482788, -0.282130, -0.110699),
    vec3(-0.201271, -0.103539, -0.926973),
    vec3(-0.595974, -0.542705, -0.669036),
    vec3(0.794496, -0.479294, 0.206060),
    vec3(0.203837, 0.512669, -0.749638),
    vec3(0.109093, -0.571021, -0.795460),
    vec3(-0.141817, 0.041201, -0.426505),
    vec3(0.328846, -0.049785, 0.015148),
    vec3(0.190334, -0.023223, -0.517668),
    vec3(0.300549, 0.577648, -0.808747),
    vec3(0.495327, -0.392870, -0.301152),
    vec3(0.574698, -0.286134, 0.795659),
    vec3(0.831999, -0.328244, -0.299034),
    vec3(0.000584, 0.809287, 0.673695),
    vec3(-0.200837, 0.404456, 0.014106),
    vec3(-0.550366, -0.259364, -0.269339),
    vec3(-0.105195, 0.575651, 0.932492),
    vec3(-0.746786, 0.820868, 0.096740),
    vec3(-0.587484, -0.609622, -0.743613),
    vec3(0.783550, -0.834414, 0.399929),
    vec3(-0.456982, -0.817008, 0.938401),
    vec3(-0.926332, -0.563874, 0.124845),
    vec3(-0.155333, 0.501327, -0.761461),
    vec3(-0.159238, 0.198570, 0.212175),
    vec3(-0.778284, 0.059120, -0.285766),
    vec3(-0.550787, -0.975279, -0.415373),
    vec3(0.517587, -0.914496, -0.077145),
    vec3(0.574851, 0.207200, 0.047002),
    vec3(-0.033576, -0.324151, 0.221746),
    vec3(-0.858240, 0.158234, -0.618235),
    vec3(0.298991, -0.860200, 0.848942),
    vec3(-0.883445, -0.187100, 0.681244),
    vec3(-0.293335, 0.154231, 0.564256),
    vec3(-0.821321, -0.480535, 0.027126),
    vec3(-0.431921, -0.304866, -0.145516),
    vec3(-0.344856, -0.201791, -0.021198),
    vec3(0.080231, 0.980156, 0.014249),
    vec3(-0.930799, -0.782377, -0.828903),
    vec3(0.376979, -0.585623, 0.687978),
    vec3(0.240288, 0.660880, 0.254263)
);

highp vec3 makePerpendicular(highp vec3 v) {
    if(v.x == 0.0 && v.y == 0.0) {
        if(v.z == 0.0) {
            return vec3(0.0, 0.0, 0.0);
        }
        return vec3(0.0, 1.0, 0.0);
    }
    return vec3(-v.y, v.x, 0.0);
}

highp float linearizeDepth(highp float z) {
    highp float f=1000.0;
    highp float n = 0.1;

    return (2.0 * n) / (f + n - z * (f - n));
}

highp vec2 texCoordFromPosition(highp vec3 position, highp mat4 viewMatrix, highp mat4 projectionMatrix) {
    highp vec4 projectedPosition = projectionMatrix * viewMatrix * vec4(position.xyz, 1.0);
    projectedPosition.xy /= projectedPosition.w;
    projectedPosition.xy = projectedPosition.xy * 0.5 + 0.5;
    return projectedPosition.xy;
}

highp float ambientOcclusion(highp sampler2D depthTexture, highp sampler2D noiseTexture,
                             highp vec3 position, highp vec3 inNormal,
                       highp int samples, highp float radius, highp float noiseScale,
                       highp mat4 viewMatrix, highp  mat4 projectionMatrix) {
    highp vec2 positionTexCoord = texCoordFromPosition(position, viewMatrix, projectionMatrix);
    highp float fragDepth = linearizeDepth(texture(depthTexture, positionTexCoord).r);

    highp vec3 normal = normalize(inNormal);
    highp vec3 randomVector = normalize(-1.0 + 2.0 * texture(noiseTexture, positionTexCoord * noiseScale).rgb);

    highp vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
    highp vec3 bitangent = normalize(cross(normal, tangent));
    highp mat3 basis = mat3(tangent, bitangent, normal);

    highp float occlusion = 0.0;
    for(int i = 0; i < samples; i++) {
        highp vec3 sampleRay = normalize(basis * sphereVectors[i]);
//        highp vec3 sampleRay = normalize(basis * texture(noiseTexture, (positionTexCoord + vec2(float(i), float(i) * 0.1) * 0.1) * noiseScale).rgb);
        if(abs(dot(sampleRay, normal)) < 0.15) {
            continue;
        }
        sampleRay *= radius;
        highp vec3 samplePosition = position.xyz + sampleRay;

        highp vec2 texCoord = texCoordFromPosition(samplePosition, viewMatrix, projectionMatrix);
        highp float sampleDepth = linearizeDepth(texture(depthTexture, texCoord).r);
        if(abs(sampleDepth - fragDepth) < radius && sampleDepth < fragDepth) {
            occlusion += 1.0;
        }
    }

    return 1.0 - occlusion / float(samples);
}

highp float hemisphereAmbientOcclusion(highp sampler2D depthTexture, highp sampler2D noiseTexture,
                             highp vec3 position, highp vec3 inNormal,
                       highp int samples, highp float radius, highp float noiseScale,
                       highp mat4 viewMatrix, highp  mat4 projectionMatrix) {
    highp vec2 positionTexCoord = texCoordFromPosition(position, viewMatrix, projectionMatrix);
    highp float fragDepth = linearizeDepth(texture(depthTexture, positionTexCoord).r);

    highp vec3 normal = normalize(inNormal);
    highp vec3 randomVector = normalize(-1.0 + 2.0 * texture(noiseTexture, positionTexCoord * noiseScale).rgb);

    highp vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
    highp vec3 bitangent = normalize(cross(normal, tangent));
    highp mat3 basis = mat3(tangent, bitangent, normal);

    highp float occlusion = 0.0;
    for(int i = 0; i < samples; i++) {
        highp vec3 sampleRay = normalize(basis * hemisphereVectors[i]);
//        highp vec3 sampleRay = normalize(basis * texture(noiseTexture, (positionTexCoord + vec2(float(i), float(i) * 0.1) * 0.1) * noiseScale).rgb);
        if(abs(dot(sampleRay, normal)) < 0.15) {
            continue;
        }
        sampleRay *= radius;
        highp vec3 samplePosition = position.xyz + sampleRay;

        highp vec2 texCoord = texCoordFromPosition(samplePosition, viewMatrix, projectionMatrix);
        highp float sampleDepth = linearizeDepth(texture(depthTexture, texCoord).r);
        if(abs(sampleDepth - fragDepth) <= radius && sampleDepth < fragDepth) {
            occlusion += 1.0;
        }
    }

    return (1.0 - occlusion / float(samples));
}

