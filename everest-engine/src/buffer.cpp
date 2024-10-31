#include "renderer/buffer.h"

namespace Everest {
    Buffer::Buffer(dtype *_data, size_t _size, GLenum target)
        :_target(target){
        glGenBuffers(1, &_id);
        this->bind();
        glBufferData(_target, _size, _data, GL_STATIC_DRAW);
    }

    Buffer::~Buffer(){
        glDeleteBuffers(1, &_id);
    }

    void Buffer::bind(){
        glBindBuffer(_target, _id);
    }

    void Buffer::unbind(){
        glBindBuffer(_target, 0);
    }

    VertexBuffer::VertexBuffer(dtype *_data, size_t _size)
        :Buffer(_data, _size, GL_ARRAY_BUFFER){ }

    IndexBuffer::IndexBuffer(dtype *_data, size_t _size)
        :Buffer(_data, _size, GL_ELEMENT_ARRAY_BUFFER){ }

}
