#include "sandbox2d.h"

SandBox2D::SandBox2D(const char* name)
:Layer(name), _camController(){ 
    EV_profile_function();
}

void SandBox2D::onAttach(){
    EV_profile_function();
    color = vec4(1.f);
    _logo = createRef<Texture>("assets/textures/test.png");
    _tiles = createRef<Texture>("assets/textures/test2.png");
    _tiles->setClamp();
}

void SandBox2D::onUpdate(){
    EV_profile_function();
    _camController.onUpdate();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();

    Renderer2D::beginScene(_camController.getCamera());

    for(i32 x = 0; x <= 1; x++){
        for(i32 y = 0; y <= 1; y++){
            Renderer2D::drawQuad(vec2(x*1.1f, y*1.1f), vec2(1.f), 0.f,
                    vec4(1.f), x?_tiles:_logo, 2.f);
        }
    }

    Renderer2D::endScene();
}

void SandBox2D::onGUIrender(){
        EV_profile_function();

        ImGui::ShowDemoWindow();
        ImGui::Begin(_name);

        i32 texture_units;
        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
        ImGui::Text("Vendor         : %s", glGetString(GL_VENDOR));
        ImGui::Text("Renderer       : %s", glGetString(GL_RENDERER));
        ImGui::Text("Version        : %s", glGetString(GL_VERSION));
        ImGui::Text("TexSlot count  : %d", texture_units);

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("FPS            : %d frames/sec", Time::getFrameRate());
        ImGui::Text("Framecount     : %d frames", Time::getFrameCount());

        ImGui::Text("unscaled-time  : %.7f sec", Time::getUnscaledTime());
        ImGui::Text("unscaled-d-time: %.7f sec", Time::getUnscaledDeltatime());

        ImGui::Text("scaled-time    : %.7f sec", Time::getTime());
        ImGui::Text("scaled-d-time  : %.7f sec", Time::getDeltatime());

        static f32 _scale = 1.f;
        ImGui::SliderFloat("Time scale", &_scale, 0.f, 10.f);
        Time::setScale(_scale);

        if(ImGui::Button("Pause")) Time::setPauseState(!Time::isPaused());
        if(ImGui::Button("Manual Tick")) Time::tickManually();

        ImGui::End();
}

void SandBox2D::onEvent(Event& event){
    if(event.getEventType() == EventType::WindowResize){
        _camController.onWindowResize((WindowResizeEvent&)event);
    }
}

void SandBox2D::onDetach(){
    EV_profile_function();
}
