#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>

// Texture loading
#include "lodepng.h"

enum TextureType {
    DIFFUSION,
    SPECULAR,
    HEIGHT,
    NORMAL,
    UNDEFINED
};

class Texture
{
public:
    // Texture identifier
    GLuint Id;

    TextureType type;

    std::string path;

    // Create new texture from image, setup wrapping and filtering parameters
    Texture(const GLchar* textureFile, GLint sWrap, GLint tWrap, GLint minFilter, GLint magFilter, TextureType type = UNDEFINED);

    // Bind texture for use
    void Bind();
};
#endif
