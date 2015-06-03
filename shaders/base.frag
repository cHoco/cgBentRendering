#version 330 core
in vec3 ourColor;
in vec2 ourTexPosition;

uniform sampler2D ourTexture;

out vec4 color;

void main()
{
    /* color = mix(vec4(ourColor, 1.0), texture(ourTexture, ourTexPosition), 0.2); */
    color = texture(ourTexture, ourTexPosition);
}
