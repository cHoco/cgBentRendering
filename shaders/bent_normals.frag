#version 330 core

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D depthTexture;
uniform sampler2D uNoiseTex;

const int MAX_KERNEL_SIZE = 128;
uniform int uKernelSize;
uniform vec3 uKernelOffsets[MAX_KERNEL_SIZE];
uniform float uRadius = 1.5;
uniform float uPower = 2.0;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

in vec2 TexCoords;

out vec4 outBNAO;

vec3 encodeNormal(const in vec3 normal) {
    return normal * 0.5 + 0.5;
}

vec3 decodeNormal(const in vec3 normal) {
    return normal * 2.0 - 1.0;
}

float linearizeDepth(in float depth, in mat4 projMatrix) {
    return projMatrix[3][2] / (-2.0*depth+1.0 - projMatrix[2][2]);
}

void main() {
    vec4 wsPosition = texelFetch(positionTexture, ivec2(gl_FragCoord.xy), 0);
    vec3 csPosition = (viewMatrix * vec4(wsPosition.xyz, 1.0)).xyz;

    vec3 normal = decodeNormal(texelFetch(normalTexture, ivec2(gl_FragCoord.xy), 0).xyz);
    normal = normalize(normal);

    vec3 rvec = texture(uNoiseTex, TexCoords * uNoiseScale).xyz * 2.0 - 1.0;
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;
    for (int i = 0; i < uSampleKernelSize; ++i) {
        // get sample position:
        vec3 samplePos = tbn * uKernelOffsets[i];
        samplePos = samplePos * uRadius + csPosition;

        // project sample position:
        vec4 offset = vec4(samplePos, 1.0);
        offset = projectionMatrix * offset;
        offset.xy /= offset.w;
        offset.xy = offset.xy * 0.5 + 0.5;

        // get sample depth:
        float sampleDepth = texelFetch(depthTexture, ivec2(offset.xy), 0).x;
        sampleDepth = -linearizeDepth(sampleDepth, projectionMatrix);

        // range check & accumulate:
        float rangeCheck = smoothstep(0.0, 1.0, uRadius / abs(csPosition.z - sampleDepth));
        occlusion += rangeCheck * step(sampleDepth, samplePos.z);
    }

    occlusion = 1.0 - (occlusion / float(uKernelSize));
    outBNAO = vec4(pow(occlusion, uPower));
}
