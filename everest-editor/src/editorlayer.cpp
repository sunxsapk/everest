#include "editorlayer.h"
#include "editor/editorcamera.h"
#include "editor/statspanel.h"
#include "editor/menupanel.h"


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
    _activeScene = createRef<Scene>();
    _scenehui.setScene(_activeScene);
    _camera = _activeScene->createEntity("Scene Camera");
    _camera.add<camera_c>(camera_c{OrthographicCamera(9.f, 16.f/9.f)});
    _camera.add<nativeScript_c>().bind<CameraController>();

    Entity e = _activeScene->createEntity();
    e.add<spriteRenderer_c>(spriteRenderer_c{
            .sprite = _farmsprites.getSprite({0, 0}, {1, 1}),
            });
}

void EditorLayer::onUpdate(){
    EV_profile_function();

    _framebuffer->bind();

    Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
    Renderer::issue_clear();
    Renderer2D::beginScene(_camera.get<camera_c>(), _camera.get<transform_c>());

    _activeScene->onUpdate();

    Renderer2D::endScene();

    _framebuffer->unbind();
}

void EditorLayer::onGUIrender(){
        EV_profile_function();
        //ImGui::ShowDemoWindow();

        MenuPanel::onGuiRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        _scenehui.onGUIrender();
        _props.onGUIrender(_scenehui.getSelectedEntity());

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
        _activeScene->onViewportResize(_sceneViewPortSize);
    }

    ImGui::Image(_framebuffer->getColorAttachment(),
            ImVec2(_sceneViewPortSize.x, _sceneViewPortSize.y), uv0, uv1);

    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorLayer::onEvent(Event& event){
}

void EditorLayer::onDetach(){
    EV_profile_function();
}
