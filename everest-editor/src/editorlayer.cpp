#include "editorlayer.h"
#include "editor/editorcamera.h"
#include "editor/statspanel.h"
#include "editor/menupanel.h"
#include "editor/propspanel.h"
#include "editor/sceneheirarchy.h"
#include "editor/scenepanel.h"

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

    void EditorLayer::onDetach(){
        EV_profile_function();
    }

    void EditorLayer::onEvent(Event& event){
        EventDispatcher dispatcher(event);

        dispatcher.dispatch<KeyDownEvent>(MenuPanel::onKeyShortcuts);
    }

    void EditorLayer::onGUIrender(){
        EV_profile_function();
        ImGui::ShowDemoWindow();

        MenuPanel::onGuiRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        SceneHeirarchyUI::onGUIrender();
        PropertiesPanel::onGUIrender(SceneHeirarchyUI::getSelectedEntity());
        ScenePanel::onGUIrender(_framebuffer, _camera);
    }
}
