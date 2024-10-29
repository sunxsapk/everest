/*
 * ========== buffer ===========
 * Author: Sunil Sapkota
 * Description: buffer class and its childs provide the wrapper around opengl's
 * buffers. some of the buffers implementation are vertex buffers, element
 * buffers, etc.
 */

#pragma once

namespace Everest {
    using dtype = const void;
    class Buffer {
        public:
            ~Buffer();

            virtual void bind();
            virtual void unbind();
            void setData(dtype *_data, size_t _size,
                    GLenum _usage = GL_STATIC_DRAW);
        protected:
            u32 _id;
            GLenum _target;

            Buffer();
    };

    class VertexBuffer : public Buffer {
        public:
            VertexBuffer();
    };

    class ElementBuffer : public Buffer {
        public:
            ElementBuffer();
    };
}
