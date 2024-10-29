#include "renderer/vao.h"
#include "math/utils.h"

namespace Everest{
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

    void VAO::layout(u32 count, GLenum type, u32 stride){
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
