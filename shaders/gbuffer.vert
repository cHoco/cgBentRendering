in vec3 position;
in vec3 normal;
in vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourPosition;
out vec3 ourNormal;
out vec3 ourTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    ourPosition = vec3(model * vec4(position, 1.0));
    ourNormal = mat3(transpose(inverse(model))) * normal;
    ourTexCoord = texCoord;
}
