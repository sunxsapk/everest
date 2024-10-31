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

            /* binds and starts the layout specification for current vertex
             * array */
            void beginLayout();
            /*updates the layout of the vertex array object with given data:-
              @params:-
                @count : number of the datatypes in the layout
                @type  : Enum specifying the primitive used in layout
                @stride: gap between 2-same field on consecutive data buffer, or
                    size of single item in the buffer

                @Only following datatypes are valid:
                    GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT,
                    GL_INT, GL_UNSIGNED_INT, GL_FIXED, GL_HALF_FLOAT,
                    GL_FLOAT, GL_DOUBLE,
             */
            void layout(u32 count, GLenum type, size_t stride);
            /*ends the layout specification (basically, unbinds the vertex
             * array object)*/
            void endLayout();

            /* binds the current vertex array object*/
            void bind();
            /* unbinds the current vertex array object*/
            void unbind();
        private:
            u32 _id, _index;
            u64  _offset;
    };
}
