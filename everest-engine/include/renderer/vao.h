/*
 * ======= vao ========
 * Author: Sunil Sapkota
 * Description: vertex array objects provided by opengl api are wrapped around
 * to tailor for use case in the engine.
 */

#pragma once
#include "buffer.h"

namespace Everest{
    class VAO {
        public:
            VAO();
            ~VAO();

            /* adds & binds the vertex buffer to vertex array object */
            void addVertexBuffer(p_shared(VertexBuffer) vertexBuf);
            /* adds & binds the index buffer to vertex array object */
            void addIndexBuffer(p_shared(IndexBuffer) index);
            /* returns the shared pointer to the vertex buffer added to the
             * vertex array object. Returns NULL is there is no vertex buffer
             * added */
            inline p_shared(VertexBuffer) getVertexBuffer(){return _vertexBuf;}
            /* returns the shared pointer to the vertex buffer added to the
             * vertex array object. Returns NULL is there is no vertex buffer
             * added */
            inline p_shared(IndexBuffer) getIndexBuffer(){return _indexBuf;}

            /* binds the current vertex array object*/
            void bind();
            /* unbinds the current vertex array object*/
            void unbind();

        private:
            /* sets the layout of the buffer */
            void setLayout(const BufferLayout& layout);
        private:
            u32 _id;
            p_shared(VertexBuffer) _vertexBuf;
            p_shared(IndexBuffer)  _indexBuf;
    };
}
