#include "renderer/renderAPI.h"

namespace Everest {
    void RenderAPI::clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderAPI::setClearColor(vec4 color){
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderAPI::drawIndexed(ref<VAO>& vertexArray, u32 indexCount){
        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, indexCount?indexCount
                :vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, NULL);
        vertexArray->unbind();
    }

    void RenderAPI::drawLines(ref<VAO>& vertexArray, u32 vertexCount){
        vertexArray->bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }
     
}
