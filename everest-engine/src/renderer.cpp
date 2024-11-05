#include "renderer/renderer.h"

namespace Everest {
    RenderAPI *Renderer::_api = NULL;
    SceneData Renderer::_scene{mat4(0)};

    void Renderer::init(){
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ASSERT(_api == NULL, "Api already created");
        _api = new RenderAPI();
    }

    void Renderer::quit(){
        ASSERT(_api != NULL, "No API to quit");
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
