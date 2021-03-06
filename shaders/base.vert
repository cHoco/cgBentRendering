#version 330 core

in vec3 position;
in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourNormal;
out vec3 ourFrag;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    ourFrag = vec3(model * vec4(position, 1.0));
    ourNormal = mat3(transpose(inverse(model))) * normal;
}
