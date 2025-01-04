/*
 * ======= vao ========
 * Author: Sunil Sapkota
 * Description: vertex array objects provided by opengl api are wrapped around
 * to tailor for use case in the engine.
 */

#pragma once
#include "buffer.h"
#include "core/utils.h"
#include "math/types.h"

namespace Everest{
    class VAO {
        public:
            VAO();
            ~VAO();

            /* adds & binds the vertex buffer to vertex array object */
            void addVertexBuffer(ref<VertexBuffer> vertexBuf);
            /* adds & binds the index buffer to vertex array object */
            void addIndexBuffer(ref<IndexBuffer> index);
            /* returns the shared pointer to the vertex buffer added to the
             * vertex array object. Returns NULL if there is no vertex buffer
             * added */
            inline ref<VertexBuffer> getVertexBuffer(){return _vertexBuf;}
            /* returns the shared pointer to the vertex buffer added to the
             * vertex array object. Returns NULL is there is no vertex buffer
             * added */
            inline ref<IndexBuffer> getIndexBuffer(){return _indexBuf;}

            /* binds the current vertex array object*/
            void bind();
            /* unbinds the current vertex array object*/
            void unbind();

        private:
            /* sets the layout of the buffer */
            void setLayout(const BufferLayout& layout);
        private:
            u32 _id;
            ref<VertexBuffer> _vertexBuf;
            ref<IndexBuffer>  _indexBuf;
    };
}
