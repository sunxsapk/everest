#include "renderer/renderer.h"

namespace Everest {
    RenderAPI *Renderer::_api = NULL;
    SceneData Renderer::_scene{mat4(0)};

    void Renderer::init(){
        ASSERT(_api == NULL);
        _api = new RenderAPI();
    }

    void Renderer::quit(){
        ASSERT(_api != NULL);
        delete _api;
    }

    void Renderer::beginScene(Camera& camera){
        _scene.viewProjectionMatrix = camera.getVPmatrix();
    }

    void Renderer::endScene(){
    }

    void Renderer::submit(ref<VAO>& vertexArray, ref<Shader>& shader,
            mat4 transform){
        shader->bind();
        shader->setUniform_Mat4("u_viewProjMat", _scene.viewProjectionMatrix);
        shader->setUniform_Mat4("u_transform", transform);
        _api->drawIndexed(vertexArray);
    }
}
