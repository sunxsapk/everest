#include "renderer/framebuffer.h"

namespace Everest {
    Framebuffer::Framebuffer(const FramebufferSpecs& specs)
    :_specs(specs){
        EV_profile_function();

        glGenFramebuffers(1, &_id);
        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        glGenTextures(1, &_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, _colorAttachment);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _specs.width, _specs.height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                _colorAttachment, 0);

        glGenTextures(1, &_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, _depthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, _specs.width, _specs.height);
        //glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, _specs.width,
        //        _specs.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                _depthAttachment, 0);

        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
                "Framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    Framebuffer::~Framebuffer(){
        EV_profile_function();
        glDeleteFramebuffers(1, &_id);
    }

    void Framebuffer::bind(){
        EV_profile_function();
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
    }
    void Framebuffer::unbind(){
        EV_profile_function();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
