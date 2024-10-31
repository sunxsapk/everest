#include "renderer/vao.h"
#include "math/utils.h"

namespace Everest{
    size_t glEnum2Size(GLenum _enum){
        switch(_enum){
						case GL_BYTE:
                return sizeof(GLbyte);
						case GL_UNSIGNED_BYTE:
                return sizeof(GLubyte);
						case GL_SHORT:
                return sizeof(GLshort);
						case GL_UNSIGNED_SHORT:
                return sizeof(GLushort);
						case GL_INT:
                return sizeof(GLint);
						case GL_UNSIGNED_INT:
                return sizeof(GLuint);
						case GL_FIXED:
                return sizeof(GLfixed);
						case GL_HALF_FLOAT:
                return sizeof(GLhalf);
						case GL_FLOAT:
                return sizeof(GLfloat);
						case GL_DOUBLE:
                return sizeof(GLdouble);
        }
        return 0;
    }

    VAO::VAO(){
        glGenVertexArrays(1, &_id);
    }

    VAO::~VAO(){
        glDeleteVertexArrays(1, &_id);
    }

    void VAO::beginLayout(){
        this->_offset = 0;
        this->_index = 0;
        this->bind();
    }

    void VAO::endLayout(){
        this->unbind();
    }

    void VAO::layout(u32 count, GLenum type, size_t stride){
        glEnableVertexAttribArray(this->_index);
        glVertexAttribPointer(this->_index, count, type, GL_FALSE,
                stride, (void*)this->_offset);
        this->_index++;
        this->_offset += count * glEnum2Size(type);
    }

    void VAO::bind(){
        glBindVertexArray(_id);
    }

    void VAO::unbind(){
        glBindVertexArray(0);
    }
}
