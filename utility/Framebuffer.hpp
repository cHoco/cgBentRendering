#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Ein.hpp"
#include "Texture.hpp"

#include <vector>
#include <string>
using namespace::std;

enum TexAttachmentType {
    RGB,
    RGBA,
    DEPTH,
    STENCIL
};

struct TexAttachment {
    GLuint ID;
    TexAttachmentType type;
    Size size;
    string tag;
};

class Framebuffer
{
public:
    GLuint fbID;

    Framebuffer();
    ~Framebuffer();

    void setDrawnColorBuffers(GLenum buf);
    void addTextureAttachment(TexAttachmentType type, Size texSize, string tag = "");
    void setupMRT();

    Texture getTextureAttachment(TexAttachmentType type);
    Texture getTextureAttachment(string type);

    void BindFb();
    void UnbindFb();

    bool isReady();
private:
    vector<TexAttachment> textureAttachments;
    int colorAttachmentsNum = 0;
    bool isBinded = false;
};
#endif
