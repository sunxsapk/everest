#include "editorlayer.h"
#include "editor/editorcamera.h"
#include "editor/statspanel.h"
#include "editor/menupanel.h"
#include "editor/propspanel.h"
#include "editor/sceneheirarchy.h"
#include "editor/scenepanel.h"
#include "editor/gizmos.h"

namespace Everest {

    EditorLayer::EditorLayer(const char* name)
        :Layer(name){ 
        EV_profile_function();
    }

    void EditorLayer::onAttach(){
        EV_profile_function();

        FramebufferSpecs specs{
            .width = 1280,
                .height = 720
        };
        _framebuffer = createRef<Framebuffer>(specs);
        Gizmos::init();
    }

    void EditorLayer::onUpdate(){
        EV_profile_function();

        _framebuffer->bind();

        _camera.onUpdate(ScenePanel::isFocused());

        Renderer::issue_setClearColor(ScenePanel::sceneBackgroundColor);
        Renderer::issue_clear();

        Renderer::disableDepth();
        Gizmos::renderGrid(_camera);

        Renderer::enableDepth();
        auto _activeScene = SceneManager::getActiveScene();
        if(_activeScene){
            Renderer2D::beginScene(_camera.camera, _camera.getTransform());
            _activeScene->onUpdate();
            Renderer2D::endScene();
        }

        _framebuffer->unbind();
    }

    void EditorLayer::onDetach(){
        EV_profile_function();

        Gizmos::quit();
    }

    void EditorLayer::onEvent(Event& event){
        EventDispatcher dispatcher(event);

        dispatcher.dispatch<KeyDownEvent>(MenuPanel::onKeyShortcuts);
    }

    void EditorLayer::onGUIrender(){
        EV_profile_function();
        //ImGui::ShowDemoWindow();

        MenuPanel::onGuiRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        SceneHeirarchyUI::onGUIrender();
        PropertiesPanel::onGUIrender(SceneHeirarchyUI::getSelectedEntity());
        ScenePanel::onGUIrender(_framebuffer, _camera);
    }
}
