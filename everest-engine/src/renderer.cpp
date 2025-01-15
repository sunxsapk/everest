#include "renderer/renderer.h"
#include "renderer/renderer2d.h"

namespace Everest {
    SceneData Renderer::_scene{mat4(0)};

    void Renderer::init(){
        EV_profile_function();

        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Renderer2D::init();
    }

    void Renderer::quit(){
        EV_profile_function();

        Renderer2D::quit();
    }

    void Renderer::beginScene(camera_c& camera){
        EV_profile_function();

        _scene.viewProjectionMatrix = camera.getProjection();
    }

    void Renderer::endScene(){
        EV_profile_function();

    }

    void Renderer::submit(ref<VAO>& vertexArray, ref<Shader>& shader,
            mat4 transform){
        EV_profile_function();

        shader->bind();
        shader->setUniform_Mat4("u_viewProjMat", _scene.viewProjectionMatrix);
        shader->setUniform_Mat4("u_transform", transform);
        RenderAPI::drawIndexed(vertexArray);
    }
}
