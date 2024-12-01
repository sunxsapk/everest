#include "editorlayer.h"
#include "editor/editorcamera.h"
#include "editor/statspanel.h"
#include "editor/menupanel.h"
#include "editor/propspanel.h"
#include "editor/sceneheirarchy.h"

namespace Everest {

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
    }

    void EditorLayer::onUpdate(){
        EV_profile_function();

        _framebuffer->bind();

        Renderer::issue_setClearColor({.1f, .1f, .1f, 1.f});
        Renderer::issue_clear();

        _camera.onUpdate();
        auto _activeScene = SceneManager::getActiveScene();
        if(_activeScene){
            Renderer2D::beginScene(_camera.getCamera(), _camera.getTransform());
            _activeScene->onUpdate();
            Renderer2D::endScene();
        }


        _framebuffer->unbind();
    }

    void EditorLayer::onGUIrender(){
        EV_profile_function();
        ImGui::ShowDemoWindow();

        MenuPanel::onGuiRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        SceneHeirarchyUI::onGUIrender();
        PropertiesPanel::onGUIrender(SceneHeirarchyUI::getSelectedEntity());

        handleSceneViewPort();
    }

    void EditorLayer::handleSceneViewPort() {
        static ImVec2 uv0{0.f, 1.f}, uv1{1.f, 0.f};

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene");

        _sceneViewportFocused = ImGui::IsWindowFocused();

        ImVec2 _svps_i = ImGui::GetContentRegionAvail();
        uvec2 _svps{_svps_i.x, _svps_i.y};
        bool needResize = false;
        if(_svps != _sceneViewPortSize){
            _sceneViewPortSize = _svps;
            needResize = true;
        }

        ImGui::Image(_framebuffer->getColorAttachment(),
                ImVec2(_sceneViewPortSize.x, _sceneViewPortSize.y), uv0, uv1);

        if(needResize){
            //_framebuffer->resize(_sceneViewPortSize); TODO
            _camera.onViewportResize(_sceneViewPortSize);
            auto _activeScene = SceneManager::getActiveScene();
            if(_activeScene) _activeScene->onViewportResize(_sceneViewPortSize);
        }

        ImGui::End();
        ImGui::PopStyleVar();
    }
    void EditorLayer::onDetach(){
        EV_profile_function();
    }

    void EditorLayer::onEvent(Event& event){
        EventDispatcher dispatcher(event);

        dispatcher.dispatch<KeyDownEvent>(MenuPanel::onKeyShortcuts);
    }

}
