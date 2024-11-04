#include "renderer/vao.h"
#include "math/utils.h"

namespace Everest{

    VAO::VAO(){
        glGenVertexArrays(1, &_id);
    }

    VAO::~VAO(){
        glDeleteVertexArrays(1, &_id);
    }

    void VAO::addVertexBuffer(ref<VertexBuffer> vertexBuf){
        glBindVertexArray(_id);
        vertexBuf->bind();
        setLayout(vertexBuf->getLayout());
        glBindVertexArray(0);
        _vertexBuf = vertexBuf;
    }

    void VAO::addIndexBuffer(ref<IndexBuffer> indexBuf){
        glBindVertexArray(_id);
        indexBuf->bind();
        glBindVertexArray(0);
        _indexBuf = indexBuf;
    }

    void VAO::setLayout(const BufferLayout& layout){
        ASSERT(layout.getSize() != 0);

        u32 index = 0;
        for(const auto& item:layout){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, item.getCount(), item.getPrimitive(),
                    GL_FALSE, layout.getSize(), (const void*)(u64)item.offset);
            index++;
        }
    }

    void VAO::bind(){
        glBindVertexArray(_id);
    }

    void VAO::unbind(){
        glBindVertexArray(0);
    }

}
