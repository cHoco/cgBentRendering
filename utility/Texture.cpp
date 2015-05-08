#include "Texture.hpp"

Texture::Texture(const GLchar* textureFile, GLint sWrap, GLint tWrap, GLint minFilter, GLint magFilter) {

    // 1- Generate and bind texture
    glGenTextures(1, &(this->Id));
    glBindTexture(GL_TEXTURE_2D, this->Id);

    // 2- Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);

    // 3- Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    // 4- Load texture image, link it to the texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image(textureFile, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 5- Clean up memory and unbind the texture
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->Id);
}
