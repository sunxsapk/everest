#include "editorlayer.h"
#include "editor/editorcamera.h"
#include "editor/statspanel.h"
#include "editor/menupanel.h"
#include "editor/propspanel.h"
#include "editor/sceneheirarchy.h"
#include "editor/scenepanel.h"
#include "editor/gizmos.h"
#include "editor/contentBrowserPanel.h"
#include "editor/editorAssets.h"

namespace Everest {

    EditorLayer::EditorLayer(const char* name)
        :Layer(name){ 
        EV_profile_function();
    }

    void EditorLayer::onAttach(){
        EV_profile_function();

        FramebufferSpecs specs{
            .width = 1280,
            .height = 720,
            .attachments = {FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::RED_INT, FrameBufferTextureFormat::DEPTH24STENCIL8},
        };
        _framebuffer = createRef<Framebuffer>(specs);
        SceneManager::onSceneChanged([](ref<Scene>& scene){
                SceneHeirarchyUI::setScene(scene);
        });
        SceneManager::loadScene("assets/scenes/physics2d.everest");
        EditorAssets::init();
        Gizmos::init();
    }

    void EditorLayer::onUpdate(){
        EV_profile_function();

        _framebuffer->bind();

        _camera.onUpdate(ScenePanel::isFocused());

        Renderer::issue_setClearColor(ScenePanel::sceneBackgroundColor);
        Renderer::issue_clear();

        _framebuffer->clearAttachment(1, -1);
        auto _activeScene = ScenePanel::getScene();
        if(_activeScene){
            switch(ScenePanel::getSceneState()){
                case SceneState::EDIT:
                    _activeScene->onEditorRender(_camera.camera, _camera.transform);
                    //Renderer::disableDepth();
                    _framebuffer->setDrawBufferTargetCount(1); // render grid on only first one
                    Gizmos::renderGrid(_camera);
                    _framebuffer->setDrawBufferTargetCount(-1); // reset render targets
                    //Renderer::enableDepth();
                    ScenePanel::mousePickCheck(_framebuffer);
                    break;
                case SceneState::PLAY:
                    _activeScene->onUpdate();
                    _activeScene->onRender();
                    break;
            }
        }

        _framebuffer->unbind();
    }

    void EditorLayer::onDetach(){
        EV_profile_function();

        Gizmos::quit();
        EditorAssets::quit();
    }

    void EditorLayer::onEvent(Event& event){
        EventDispatcher dispatcher(event);

        dispatcher.dispatch<KeyDownEvent>(MenuPanel::onKeyShortcuts);
        dispatcher.dispatch<MouseButtonDownEvent>(BIND_EVENT_CB(onMouseButtonDown));
    }

    void EditorLayer::onGUIrender(){
        EV_profile_function();
        //ImGui::ShowDemoWindow();

        MenuPanel::onGUIRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        SceneHeirarchyUI::onGUIrender();
        PropertiesPanel::onGUIrender(SceneHeirarchyUI::getSelectedEntity());
        ScenePanel::onGUIrender(_framebuffer, _camera);
        ContentBrowser::onGUIrender();
    }

    bool EditorLayer::onMouseButtonDown(MouseButtonDownEvent& event){
        if(event.getButton() == MouseButton_0){
            _framebuffer->bind();
            ScenePanel::mousePickCheck(_framebuffer);
            _framebuffer->unbind();
            return true;
        }
        return false;
    }
}
