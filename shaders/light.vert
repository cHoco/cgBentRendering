#version 330 core

in vec3 position;
/* in vec3 color; */
in vec2 texPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor;
out vec2 ourTexPosition;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    /* ourColor = color; */
    ourTexPosition = vec2(texPosition.x, 1.0-texPosition.y);
}
