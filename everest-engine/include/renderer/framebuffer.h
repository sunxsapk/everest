/*
 * ============ Framebuffer ==============
 * Author: Sunil Sapkota
 * Description: Framebuffers are basically the textures which are created on the
 * fly by using them as a rendering canvas for the scene. They are used in same
 * way one uses textures. They can be used for postprocessing stuffs,
 * shadow mapping, etc.
 */

#pragma omce
#include "math/types.h"

namespace Everest {
    struct FramebufferSpecs {
        u32 width, height, samples=1;
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

            inline const u32 getColorAttachment(){return _colorAttachment;}
            inline const u32 getDepthAttachment(){return _depthAttachment;}

            inline const FramebufferSpecs& getSpecs(){return _specs;}

        private:
            void invalidate();
        private:
            FramebufferSpecs _specs;
            u32 _id, _colorAttachment, _depthAttachment;
    };
}
