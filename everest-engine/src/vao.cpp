#include "renderer/vao.h"
#include "math/utils.h"

namespace Everest{

    VAO::VAO(){
        EV_profile_function();
        glGenVertexArrays(1, &_id);
    }

    VAO::~VAO(){
        EV_profile_function();
        glDeleteVertexArrays(1, &_id);
    }

    void VAO::addVertexBuffer(ref<VertexBuffer> vertexBuf){
        EV_profile_function();

        glBindVertexArray(_id);
        vertexBuf->bind();
        setLayout(vertexBuf->getLayout());
        glBindVertexArray(0);
        _vertexBuf = vertexBuf;
    }

    void VAO::addIndexBuffer(ref<IndexBuffer> indexBuf){
        EV_profile_function();

        glBindVertexArray(_id);
        indexBuf->bind();
        glBindVertexArray(0);
        _indexBuf = indexBuf;
    }

    void VAO::setLayout(const BufferLayout& layout){
        EV_profile_function();

        ASSERT(layout.getSize() != 0, "Layout not specified");

        u32 index = 0;
        for(const auto& item:layout){
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index, item.getCount(), item.getPrimitive(),
                    GL_FALSE, layout.getSize(), (const void*)(u64)item.offset);
            index++;
        }
    }

    void VAO::bind(){
        EV_profile_function();

        glBindVertexArray(_id);
    }

    void VAO::unbind(){
        EV_profile_function();

        glBindVertexArray(0);
    }

}
