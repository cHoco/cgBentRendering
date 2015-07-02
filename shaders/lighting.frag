#version 330 core

in vec2 TexCoords;

uniform sampler2D tPosition;
uniform sampler2D tNormals;
uniform sampler2D tDiffuse;
uniform sampler2D tBentNormals;

uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec4 color;

vec3 decodeNormal(const in vec3 normal) {
    return normal * 2.0 - 1.0;
}

void main()
{
    vec3 image = texelFetch(tDiffuse, ivec2(gl_FragCoord.xy), 0).xyz;
    vec3 position = texelFetch(tPosition, ivec2(gl_FragCoord.xy), 0).xyz;
    vec3 normal = texelFetch(tNormals,  ivec2(gl_FragCoord.xy), 0).xyz;
    vec3 bnAo = texelFetch(tBentNormals, ivec2(gl_FragCoord.xy), 0).xyz;
    vec3 bentNormal = decodeNormal(bnAo) + normal;

    // ambient
    vec3 ambient = 0.6 * image;

    // diffuse
    vec3 lightDir = lightPos - position;
    normal = normalize(normal);
    /* normal = normalize(bentNormal); */
    lightDir = normalize(lightDir);
    float diff = clamp(dot(lightDir, normal), 0.0, 1.0);
    vec3 diffuse = diff * image;

    // spec
    vec3 viewDir = normalize(viewerPos-position);
    vec3 halfwayDir = normalize(lightDir+viewDir);
    float spec = 0.0;
    spec = pow(clamp(dot(normal, halfwayDir), 0.0, 1.0), 32.0);
    vec3 specular = vec3(0.3) * spec;
    color = vec4(ambient + diffuse + specular, 1.0f);
}

