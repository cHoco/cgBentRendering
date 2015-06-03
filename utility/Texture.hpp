#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>

// Texture loading
#include "lodepng.h"

class Texture
{
public:
    // Texture identifier
    GLuint Id;

    // Create new texture from image, setup wrapping and filtering parameters
    Texture(const GLchar* textureFile, GLint sWrap, GLint tWrap, GLint minFilter, GLint magFilter);

    // Bind texture for use
    void Bind();
};
#endif
