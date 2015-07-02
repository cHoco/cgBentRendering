#include "Texture.hpp"

Texture::Texture(const GLchar* textureFile, GLint sWrap, GLint tWrap, GLint minFilter, GLint magFilter, TextureType type) {

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
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, textureFile);

    // If there's an error, display it.
    if(error != 0)
    {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
    glGenerateMipmap(GL_TEXTURE_2D);

    // 5- Clean up memory and unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    this->type = type;
    this->path = std::string(textureFile);
}

Texture::Texture(GLuint texID) {
    this->Id = texID;
    this->path = "";
    this->type = UNDEFINED;
}

Texture::Texture() {
    glGenTextures(1, &(this->Id));
    this->path = "";
    this->type = UNDEFINED;
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, this->Id);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
