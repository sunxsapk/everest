#include "renderer/renderAPI.h"

namespace Everest {
    void RenderAPI::clear(){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderAPI::setClearColor(vec4 color){
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderAPI::drawIndexed(p_shared(VAO) vertexArray){
        vertexArray->bind();
        glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(),
                GL_UNSIGNED_INT, NULL);
        vertexArray->unbind();
    }
}
