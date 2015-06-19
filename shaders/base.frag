#version 330 core

in vec3 ourNormal;
in vec3 ourFrag;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec4 color;

void main()
{
    // ambient
    float ambientLightStrenght = 0.2f;
    vec3 ambient = ambientLightStrenght * lightColor;

    // diffuse
    vec3 normal = normalize(ourNormal);
    vec3 lightDir = normalize(lightPos - ourFrag);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewerPos - ourFrag);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    vec3 specular = specularStrength * spec * lightColor;

    // result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result , 1.0f);
}
