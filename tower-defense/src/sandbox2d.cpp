#include "sandbox2d.h"

SandBox2D::SandBox2D(const char* name)
:Layer(name), _camController(){ }

void SandBox2D::onAttach(){
    color = vec4(1.f);
    _logo = createRef<Texture>("assets/textures/test.png");
    _tiles = createRef<Texture>("assets/textures/test2.png");
    _tiles->setClamp();
}

void SandBox2D::onUpdate(){
    _camController.onUpdate();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();
    Renderer2D::beginScene(_camController.getCamera());
    Renderer2D::drawQuad(vec3(0.f), vec2(4.f), 10 * Time::getScaledTime(),
            color, _logo);
    Renderer2D::drawQuad(vec2(-4.f, -2.f), vec2(3.f, 2.f), 0.f, vec4(1.f),
            _tiles);
    Renderer2D::drawQuad(vec2(2.f), vec2(10.f, 0.3f), 10.f,
            vec4(0.3, 1.f, 0.4f, 1.f));
    Renderer2D::endScene();
}

void SandBox2D::onDebugRender(){
    ImGui::Begin(this->_name);
    ImGui::Text("Hello from renderer2d test sandbox");
    ImGui::SliderFloat4("Color", glm::value_ptr(color), 0.f, 1.f);
    ImGui::End();
}

void SandBox2D::onEvent(Event& event){
}

void SandBox2D::onDetach(){
}
