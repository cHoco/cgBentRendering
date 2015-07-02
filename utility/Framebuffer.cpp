#include "Framebuffer.hpp"

Framebuffer::Framebuffer() {
    glGenFramebuffers(1, &fbID);
}

Framebuffer::~Framebuffer() {
    glDeleteFramebuffers(1, &fbID);
}

bool Framebuffer::isReady() {
    if(isBinded) {
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
            return true;
    }
    return false;
}

void Framebuffer::setDrawnColorBuffers(GLenum buf) {
    if(isBinded) {
        glDrawBuffer(buf);
    }
}

void Framebuffer::BindFb() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbID);
    isBinded = true;
}

void Framebuffer::UnbindFb() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    isBinded = false;
}

Texture Framebuffer::getTextureAttachment(TexAttachmentType type) {
    for(std::vector<TexAttachment>::iterator it = textureAttachments.begin(); it != textureAttachments.end(); ++it) {
        if((*it).type == type) {
            Texture temp((*it).ID);
            temp.type = TextureType::BUFFER_ATTACHMENT;
            return temp;
        }
    }
    return NULL;
}

Texture Framebuffer::getTextureAttachment(string tag) {
    for(std::vector<TexAttachment>::iterator it = textureAttachments.begin(); it != textureAttachments.end(); ++it) {
        if((*it).tag == tag) {
            Texture temp((*it).ID);
            temp.type = TextureType::BUFFER_ATTACHMENT;
            return temp;
        }
    }
    return NULL;
}

void Framebuffer::addTextureAttachment(TexAttachmentType type, Size texSize, string tag) {
    if(isBinded) {
        TexAttachment texAttachment;
        texAttachment.type = type;
        texAttachment.size = texSize;
        texAttachment.tag = tag;
        GLenum texAttachType;
        GLenum fbAttachType;

        switch(type) {
            case RGB:
                texAttachType = GL_RGB;
                fbAttachType = GL_COLOR_ATTACHMENT0+GLenum(colorAttachmentsNum);
                colorAttachmentsNum++;
                break;
            case RGBA:
                texAttachType = GL_RGBA;
                fbAttachType = GL_COLOR_ATTACHMENT0+GLenum(colorAttachmentsNum);
                colorAttachmentsNum++;
                break;
            case DEPTH:
                texAttachType = GL_DEPTH_COMPONENT;
                fbAttachType = GL_DEPTH_ATTACHMENT;
                break;
            case STENCIL:
                texAttachType = GL_STENCIL_INDEX;
                fbAttachType = GL_STENCIL_ATTACHMENT;
                break;
            default:
                return;
        }

        glGenTextures(1, &texAttachment.ID);
        glBindTexture(GL_TEXTURE_2D, texAttachment.ID);
        if(type == DEPTH) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, texSize.width, texSize.height, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, texAttachType, texSize.width, texSize.height, 0, texAttachType, GL_FLOAT, NULL);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, fbAttachType, GL_TEXTURE_2D, texAttachment.ID, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        this->textureAttachments.push_back(texAttachment);
    }
    else {
        std::cerr << "[FRAMEBUFFER] Error trying to add texture attachments to unbinded framebuffer" << std::endl;
    }
}

void Framebuffer::setupMRT() {
    if(isBinded) {
        GLenum *drawBuffers = NULL;
        drawBuffers = new GLenum[colorAttachmentsNum];
        for(int i=0; i<colorAttachmentsNum; i++)
            drawBuffers[i] = GL_COLOR_ATTACHMENT0+GLenum(i);
        glDrawBuffers(colorAttachmentsNum, drawBuffers);
        delete [] drawBuffers;
    }
}



