/*
 * ======= vao ========
 * Author: Sunil Sapkota
 * Description: vertex array objects provided by opengl api are wrapped around
 * to tailor for use case in the engine.
 */

#pragma once

namespace Everest{
    class VAO {
        public:
            VAO();
            ~VAO();

            void beginLayout();
            void layout(u32 count, GLenum type, u32 stride);
            void endLayout();

            void bind();
            void unbind();
        private:
            u32 _id, _index;
            u64  _offset;
    };
}
