#include "renderer/renderer.h"

namespace Everest {
    RenderAPI *Renderer::_api = NULL;

    void Renderer::init(){
        ASSERT(_api == NULL);
        _api = new RenderAPI();
    }

    void Renderer::quit(){
        ASSERT(_api != NULL);
        delete _api;
    }

    void Renderer::beginScene(Camera* camera){
    }

    void Renderer::endScene(){
    }

    void Renderer::submit(p_shared(VAO) vertexArray){
        _api->drawIndexed(vertexArray);
    }
}
