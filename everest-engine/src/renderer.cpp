#include "renderer/renderer.h"
#include "renderer/renderer2d.h"

namespace Everest {
    SceneData Renderer::_scene{mat4(0)};

    void Renderer::init(){
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Renderer2D::init();
    }

    void Renderer::quit(){
        Renderer2D::quit();
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
        RenderAPI::drawIndexed(vertexArray);
    }
}
