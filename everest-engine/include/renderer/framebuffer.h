/*
 * ============ Framebuffer ==============
 * Author: Sunil Sapkota
 * Description: Framebuffers are basically the textures which are created on the
 * fly by using them as a rendering canvas for the scene. They are used in same
 * way one uses textures. They can be used for postprocessing stuffs,
 * shadow mapping, etc.
 */

#pragma once
#include "math/types.h"
#include "pch.h"

namespace Everest {
    enum class FrameBufferTextureFormat {
        None = 0,
        RED_INT,
        RGBA8,
        DEPTH24STENCIL8,
    };

    struct FrameBufferTextureSpecs {
        FrameBufferTextureFormat textureFormat;
        // TODO
        // texture wrapMode, repeat

        FrameBufferTextureSpecs():textureFormat(FrameBufferTextureFormat::None){}
        FrameBufferTextureSpecs(FrameBufferTextureFormat fmt)
            :textureFormat(fmt){}
    };

    struct FrameBufferAttachmentSpecs {
        std::vector<FrameBufferTextureSpecs> values;

        FrameBufferAttachmentSpecs(){}
        // only one depth buffer is eligible here
        FrameBufferAttachmentSpecs(std::initializer_list<FrameBufferTextureSpecs> _attchs)
        :values(_attchs){}
    };

    struct FramebufferSpecs {
        u32 width, height, samples=1;
        FrameBufferAttachmentSpecs attachments;
        bool swapchainTarget = false;
    };

    class Framebuffer {
        public:
            Framebuffer(const FramebufferSpecs& specs);
            ~Framebuffer();

            void bind();
            void unbind();

            /* resizes the framebuffer to new size */
            void resize(uvec2 size);

            i32 readPixel(u32 attIndex, i32 x, i32 y);

            void setDrawBufferTargetCount(i32 count = -1);

            inline void clearAttachment(u32 attIndex, vec4 value){
                ASSERT(attIndex < _colorAttachments.size(), "Invalid attachment index");
                ASSERT(_colorAttachmentSpecs[attIndex].textureFormat == FrameBufferTextureFormat::RGBA8,
                        "Invalid texture format for the attachment while clearing");
                glClearBufferfv(GL_COLOR, attIndex, glm::value_ptr(value));
            }

            inline void clearAttachment(u32 attIndex, i32 value){
                ASSERT(attIndex < _colorAttachments.size(), "Invalid attachment index");
                ASSERT(_colorAttachmentSpecs[attIndex].textureFormat == FrameBufferTextureFormat::RED_INT,
                        "Invalid texture format for the attachment while clearing");
                glClearBufferiv(GL_COLOR, attIndex, &value);
            }

            inline const u32 getColorAttachment(u32 index = 0){
                ASSERT(index < _colorAttachments.size(), "Attempt to get invalid color attachment");
                return _colorAttachments[index];}
            inline const u32 getDepthAttachment(){return _depthAttachment;}

            inline const FramebufferSpecs& getSpecs(){return _specs;}

        private:
            void invalidate();
        private:
            FramebufferSpecs _specs;
            u32 _id;

            std::vector<FrameBufferTextureSpecs> _colorAttachmentSpecs;
            std::vector<u32> _colorAttachments;

            FrameBufferTextureSpecs _depthAttachmentSpecs;
            u32 _depthAttachment;
    };
}
