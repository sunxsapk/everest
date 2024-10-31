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

            /*binds the buffer*/
            virtual void bind();
            /*unbinds the buffer*/
            virtual void unbind();
        protected:
            u32 _id;
            GLenum _target;
            Buffer(dtype *_data, size_t _size, GLenum target);
    };

    class VertexBuffer : public Buffer {
        public:
            /* creates, binds and fill the vertex-buffer with data. Buffer is
             * not unbinded when it finishes creating. You have to unbind it
             * later. Most probably only after unbinding the vertex array.*/
            VertexBuffer(dtype *_data, size_t _size);
    };

    class IndexBuffer : public Buffer {
        public:
            /* creates, binds and fill the index/element-buffer with data.
             * Buffer is not unbinded when it finishes creating. You have to
             * unbind it later. Most probably only after unbinding the vertex
             * array.*/
            IndexBuffer(dtype *_data, size_t _size);
    };
}
