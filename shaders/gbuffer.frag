#version 330 core

in vec3 ourPosition;
in vec3 ourNormal;
in vec2 ourTexCoord;

uniform sampler2D diffuseTexture;

out vec4 outPosition;
out vec3 outNormal;
out vec3 outDiffuse;

void main() {
    outPosition = vec4(ourPosition, 1.0);
    outNormal = normalize(ourNormal);
    tempDiffuse = texture(diffuseTexture, ourTexCoord);
    if(tempDiffuse.r = 0.0f && tempDiffuse.g = 0.0f && tempDiffuse.b = 0.0f)
        tempDiffuse = vec3(0.3, 0.3, 0.3);
    outDiffuse = tempDiffuse;
}
