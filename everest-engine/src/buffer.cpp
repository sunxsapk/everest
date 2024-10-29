#include "renderer/buffer.h"

namespace Everest {
    Buffer::Buffer(){
        glGenBuffers(1, &_id);
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

    void Buffer::setData(dtype *_data, size_t _size, GLenum _usage){
        this->bind();
        glBufferData(_target, _size, _data, _usage);
        this->unbind();
    }

    VertexBuffer::VertexBuffer(){
        _target = GL_ARRAY_BUFFER;
    }

    ElementBuffer::ElementBuffer(){
        _target = GL_ELEMENT_ARRAY_BUFFER;
    }

}
