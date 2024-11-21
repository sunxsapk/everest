#include "editorlayer.h"


EditorLayer::EditorLayer(const char* name)
:Layer(name){ 
    EV_profile_function();
}

void EditorLayer::onAttach(){
    EV_profile_function();
    _farmsprites = SpriteSheet("assets/sprites/farm.png", uvec2(16, 16));

    FramebufferSpecs specs{
        .width = 1280,
            .height = 720
    };
    _framebuffer = createRef<Framebuffer>(specs);
    _camera = _activeScene.createEntity("Scene Camera");
    _camera.add<camera_c>(camera_c{OrthographicCamera(9.f, 16.f/9.f)});

    Entity e = _activeScene.createEntity();
    e.add<spriteRenderer_c>(spriteRenderer_c{
            .sprite = _farmsprites.getSprite({0, 0}, {1, 1}),
            });
}

void EditorLayer::onUpdate(){
    EV_profile_function();

    _framebuffer->bind();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();
    Renderer2D::beginScene(_camera.get<camera_c>());

    _activeScene.onUpdate();

    Renderer2D::endScene();

    _framebuffer->unbind();
}

void EditorLayer::onGUIrender(){
        EV_profile_function();

        handleMenuBar();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
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

            ImGui::Text("Scene viewport size : %d, %d", _sceneViewPortSize.x, _sceneViewPortSize.y);

            static f32 _scale = 1.f;
            ImGui::SliderFloat("Time scale", &_scale, 0.f, 10.f);
            Time::setScale(_scale);

            if(ImGui::Button("Pause")) Time::setPauseState(!Time::isPaused());
            if(ImGui::Button("Manual Tick")) Time::tickManually();

            ImGui::End();
        }
        handleSceneViewPort();
}

void EditorLayer::handleSceneViewPort() {
    static ImVec2 uv0{0.f, 1.f}, uv1{1.f, 0.f};

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Scene");

    _sceneViewportFocused = ImGui::IsWindowFocused();

    ImVec2 _svps_i = ImGui::GetContentRegionAvail();
    uvec2 _svps{_svps_i.x, _svps_i.y};
    if(_svps != _sceneViewPortSize){
        _sceneViewPortSize = _svps;
        _framebuffer->resize(_sceneViewPortSize);
        _activeScene.onViewportResize(_sceneViewPortSize);
    }

    ImGui::Image(_framebuffer->getColorAttachment(),
            ImVec2(_sceneViewPortSize.x, _sceneViewPortSize.y), uv0, uv1);

    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorLayer::handleMenuBar(){
    if(ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {}
            ImGui::Separator();

            if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
            if (ImGui::BeginMenu("Open Recent Scene")) {
                ImGui::MenuItem("level1.evsc");
                ImGui::MenuItem("level2.evsc");
                ImGui::MenuItem("main-menu.evsc");
                ImGui::EndMenu();
            }
            ImGui::Separator();

            if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save Scene As..")) {}

            ImGui::Separator();
            if(ImGui::MenuItem("Exit")) Application::close();
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorLayer::onEvent(Event& event){
}

void EditorLayer::onDetach(){
    EV_profile_function();
}
