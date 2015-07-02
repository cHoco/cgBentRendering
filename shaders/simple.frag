#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D screenTexture;
uniform mat4 projection;

float linearizeDepth(in float depth, in mat4 projMatrix) {
    return projMatrix[3][2] / (-2.0*depth+1.0 - projMatrix[2][2]);
}

void main()
{
  /* float testc = linearizeDepth(texelFetch(tDiffuse, ivec2(gl_FragCoord.xy), 0).x, projection); */
  /* float testc = testLinear((texelFetch(screenTexture, ivec2(gl_FragCoord.xy), 0).x)); */
  /* float testc = ProvaLinearizeDepth(TexCoords); */
  float testc = texelFetch(screenTexture, ivec2(gl_FragCoord.xy), 0).x;
  testc = -linearizeDepth(testc, projection);
  /* float testc = (projection * texelFetch(screenTexture, ivec2(gl_FragCoord.xy), 0)).z; */
  color = vec4(testc, testc, testc, 1.0f);
}
