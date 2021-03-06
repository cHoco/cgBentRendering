#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourPosition;
out vec3 ourNormal;
out vec2 ourTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    ourPosition = vec3(model * vec4(position, 1.0));
    ourNormal = mat3(transpose(inverse(model))) * normal;
    ourTexCoord = texCoord;
}
