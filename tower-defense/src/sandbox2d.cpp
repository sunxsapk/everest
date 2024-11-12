#include "sandbox2d.h"

SandBox2D::SandBox2D(const char* name)
:Layer(name), _camController(8.f){ 
    EV_profile_function();
}

void SandBox2D::onAttach(){
    EV_profile_function();
    _quads = ivec2(50, 50);
    _farmsprites = SpriteSheet("assets/sprites/farm.png", uvec2(16, 16));
}

void SandBox2D::onUpdate(){
    EV_profile_function();
    _camController.onUpdate();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();

    Renderer2D::beginScene(_camController.getCamera());

    for(int y = 0; y<10; y++){
        for(int x = 0; x<13; x++){
            Renderer2D::drawSprite(_farmsprites.getSprite({x, y}, {1,1}),
                    vec3(x+0.5f, y+0.5f, 0.f));
        }
    }

    Renderer2D::endScene();
}

void SandBox2D::onGUIrender(){
        EV_profile_function();

        ImGui::ShowDemoWindow();
        {
            ImGui::Begin("Stats");

            ImGui::Text("Vendor         : %s", glGetString(GL_VENDOR));
            ImGui::Text("Renderer       : %s", glGetString(GL_RENDERER));
            ImGui::Text("Version        : %s", glGetString(GL_VERSION));

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            ImGui::Text("FPS            : %d frames/sec", Time::getFrameRate());
            ImGui::Text("Framecount     : %d frames", Time::getFrameCount());

            ImGui::End();
        }

        {
            ImGui::Begin("RendererStats");
            RendererStats stats = Renderer2D::getStats();
            ImGui::Text("QuadCount      : %d", stats.quadCount);
            ImGui::Text("VertexCount    : %d", stats.vertexCount);
            ImGui::Text("TextureCount   : %d", stats.textureCount);
            ImGui::Text("DrawCalls      : %d", stats.drawCalls);

            ImGui::End();
        }

        {
            ImGui::Begin(_name);

            ImGui::Text("unscaled-time  : %.7f sec", Time::getUnscaledTime());
            ImGui::Text("unscaled-d-time: %.7f sec", Time::getUnscaledDeltatime());

            ImGui::Text("scaled-time    : %.7f sec", Time::getTime());
            ImGui::Text("scaled-d-time  : %.7f sec", Time::getDeltatime());

            static f32 _scale = 1.f;
            ImGui::SliderFloat("Time scale", &_scale, 0.f, 10.f);
            Time::setScale(_scale);

            if(ImGui::Button("Pause")) Time::setPauseState(!Time::isPaused());
            if(ImGui::Button("Manual Tick")) Time::tickManually();

            ImGui::SliderInt2("Quads layout", glm::value_ptr(_quads), 0, 1000);

            ImGui::End();
        }
}

void SandBox2D::onEvent(Event& event){
    if(event.getEventType() == EventType::WindowResize){
        _camController.onWindowResize((WindowResizeEvent&)event);
    }
}

void SandBox2D::onDetach(){
    EV_profile_function();
}
