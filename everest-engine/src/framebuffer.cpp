#include "renderer/framebuffer.h"
#define __MAX_FRAMEBUF_SIZE 8192

namespace Everest {
    static inline GLenum getTarget(bool msampled){
        return msampled?GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D; }


    static inline bool isDepthFormat(FrameBufferTextureFormat fmt){
        switch (fmt) {
            case FrameBufferTextureFormat::DEPTH24STENCIL8: return  true;
            default: return false;
        }
    }

    static void attachColorTex(u32 id, u32 samples, GLenum internalFormat, GLenum outformat, u32 width, u32 height, int index){
        bool msampled = samples > 1;
        if(msampled){
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }else{
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, outformat, GL_UNSIGNED_BYTE, NULL);
            //TODO : from texture specs
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, getTarget(msampled), id, 0);
    }

    static void attachDepthTex(u32 id, u32 samples, GLenum internalFormat, GLenum attachmentType, u32 width, u32 height){
        bool msampled = samples > 1;
        if(msampled){
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
        }else{
            //glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
        }
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, getTarget(msampled), id, 0);
    }

    Framebuffer::Framebuffer(const FramebufferSpecs& specs)
    :_specs(specs){
        EV_profile_function();
        for(auto txtspecs : specs.attachments.values){
            if(isDepthFormat(txtspecs.textureFormat)) _depthAttachmentSpecs = txtspecs;
            else _colorAttachmentSpecs.emplace_back(txtspecs);
        }

        invalidate();
    }

    Framebuffer::~Framebuffer(){
        EV_profile_function();
        glDeleteFramebuffers(1, &_id);
        glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
        glDeleteTextures(1, &_depthAttachment);
    }

    void Framebuffer::bind(){
        EV_profile_function();
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _specs.width, _specs.height); 
    }

    void Framebuffer::unbind(){
        EV_profile_function();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::resize(uvec2 size){
        if(size.x == 0 || size.y == 0 || size.x > __MAX_FRAMEBUF_SIZE ||
                size.y > __MAX_FRAMEBUF_SIZE){
            EVLog_Wrn("Invalid framebuffer size");
            return;
        }
        _specs.width = size.x;
        _specs.height = size.y;
        
        invalidate();
    }

    i32 Framebuffer::readPixel(u32 attachmentIndex, i32 x, i32 y){
        ASSERT(x >=  0 && y >= 0 && x < _specs.width && y < _specs.height, 
                "Out of bounds Framebuffer pixel read attempt");
        ASSERT(attachmentIndex<_colorAttachments.size(),
                "Invalid color attachment read from framebuffer");
        glReadBuffer(GL_COLOR_ATTACHMENT0+attachmentIndex);
        i32 data;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &data);
        return data;
    }

    void Framebuffer::invalidate(){
        EV_profile_function();

        if(_id){
            glDeleteFramebuffers(1, &_id);
            glDeleteTextures(_colorAttachments.size(), _colorAttachments.data());
            glDeleteTextures(1, &_depthAttachment);
            _colorAttachments.clear();
            _depthAttachment = 0;
        }

        glGenFramebuffers(1, &_id);
        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        bool msampled = _specs.samples > 1;
        if(_colorAttachmentSpecs.size()){
            _colorAttachments.resize(_colorAttachmentSpecs.size());
            glGenTextures(_colorAttachments.size(), _colorAttachments.data());

            for(size_t i=0; i<_colorAttachments.size(); i++){
                glBindTexture(getTarget(msampled), _colorAttachments[i]);

                switch(_colorAttachmentSpecs[i].textureFormat){
                    case FrameBufferTextureFormat::RED_INT:
                        attachColorTex(_colorAttachments[i], _specs.samples, GL_R32I, GL_RED_INTEGER, _specs.width, _specs.height, i);
                        break;
                    case FrameBufferTextureFormat::RGBA8:
                        attachColorTex(_colorAttachments[i], _specs.samples, GL_RGBA8, GL_RGBA, _specs.width, _specs.height, i);
                        break;
                    default: break;
                }
            }
        }

        if(_depthAttachmentSpecs.textureFormat != FrameBufferTextureFormat::None){
            glGenTextures(1, &_depthAttachment);
            glBindTexture(getTarget(msampled), _depthAttachment);
            switch(_depthAttachmentSpecs.textureFormat){
                case FrameBufferTextureFormat::DEPTH24STENCIL8:
                    attachDepthTex(_depthAttachment, _specs.samples, GL_DEPTH24_STENCIL8,
                            GL_DEPTH_STENCIL_ATTACHMENT, _specs.width, _specs.height);
                default: break;
            }
        }

        setDrawBufferTargetCount(-1);
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    void Framebuffer::setDrawBufferTargetCount(i32 count){
        if(count < 0) count = _colorAttachments.size();
        if(count == 0) {
            glDrawBuffer(GL_NONE);
            return;
        }
        ASSERT(count <= _colorAttachments.size() && count <= 16, "Not enough color attachments provided for the framebuffers");
        static constexpr GLenum bufs[16] {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4,
            GL_COLOR_ATTACHMENT5,
            GL_COLOR_ATTACHMENT6,
            GL_COLOR_ATTACHMENT7,
            GL_COLOR_ATTACHMENT8,
            GL_COLOR_ATTACHMENT9,
            GL_COLOR_ATTACHMENT10,
            GL_COLOR_ATTACHMENT11,
            GL_COLOR_ATTACHMENT12,
            GL_COLOR_ATTACHMENT13,
            GL_COLOR_ATTACHMENT14,
            GL_COLOR_ATTACHMENT15,
        };
        glDrawBuffers(count, bufs);
    }
}
