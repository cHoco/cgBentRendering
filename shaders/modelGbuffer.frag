#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec3 ourPosition;
in vec3 ourNormal;
in vec2 ourTexCoord;

uniform Material mat;
uniform sampler2D normalMap;
uniform bool useNormalMap;

out vec4 outPosition;
out vec3 outNormal;
out vec3 outDiffuse;

void main() {
    outPosition = vec4(ourPosition, 1.0);
    vec3 normal;
    if (useNormalMap) {
        normal = texture(normalMap, ourTexCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    } else {
        normal = normalize(ourNormal);
    }
    outNormal = normal;
    /* outDiffuse = vec3(texture(mat.texture_diffuse1, ourTexCoord)); */
    outDiffuse = vec3(0.5, 0.5, 0.5);
}
