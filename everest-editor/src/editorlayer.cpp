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
#include "editor/projectSettings.h"

namespace Everest {

    EditorLayer::EditorLayer(const char* name)
        :Layer(name){ 
        EV_profile_function();
    }

    void EditorLayer::onAttach(){
        EV_profile_function();

        SceneManager::onSceneChanged([](ref<Scene>& scene){
                SceneHeirarchyUI::setScene(scene);
                scene->onViewportResize(ScenePanel::getSceneOffset(), ScenePanel::getSceneViewportSize());
        });

        loadLayout();
        loadProject();

        _framebuffer = createRef<Framebuffer>(FramebufferSpecs{
            .width = 1280,
            .height = 720,
            .attachments = {
                FrameBufferTextureFormat::RGBA8,
                FrameBufferTextureFormat::RED_INT,
                FrameBufferTextureFormat::DEPTH24STENCIL8
            },
        });

        ContentBrowser::init();
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

        auto& _activeScene = SceneManager::getActiveScene();
        if(_activeScene){
            switch(ScenePanel::getSceneState()){
                case SceneState::EDIT:
                    editorView(_activeScene);
                    break;
                case SceneState::PLAY:
                    {
                        if(!_activeScene) break;
                        if(ScenePanel::gameView){
                            _activeScene->onUpdate();
                            _activeScene->onRender();
                        } else {
                            editorView(_activeScene, true);
                        }
                    }
                    break;
            }
        }

        _framebuffer->unbind();
    }

    void EditorLayer::editorView(ref<Scene>& _activeScene, bool update){
        if(!_activeScene) return;

        _activeScene->onEditorBeginRender(_camera.camera, _camera.transform);
        _activeScene->onEditorRender(Gizmos::showPhysicsShapes);

        if(update) _activeScene->onUpdate();

        Entity ent = SceneHeirarchyUI::getSelectedEntity();
        if(ent.isValid()){
            transform_c& tfr = ent.get<transform_c>();
            Renderer2D::drawRect((mat4)tfr);
        }

        _activeScene->onEditorEndRender();

        //Renderer::disableDepth();
        _framebuffer->setDrawBufferTargetCount(1); // render grid on only first one
        Gizmos::renderGrid(_camera);
        _framebuffer->setDrawBufferTargetCount(-1); // reset render targets
        //Renderer::enableDepth();
        ScenePanel::mousePickCheck(_framebuffer);
    }

    void EditorLayer::onDetach(){
        EV_profile_function();

        saveLayout();
        saveProject();
        Gizmos::quit();
        EditorAssets::quit();
    }

    void EditorLayer::onEvent(Event& event){
        EventDispatcher dispatcher(event);

        dispatcher.dispatch<KeyDownEvent>(MenuPanel::onKeyShortcuts);
        dispatcher.dispatch<KeyDownEvent>(SceneHeirarchyUI::onKeyShortcuts);
        dispatcher.dispatch<KeyDownEvent>(ScenePanel::onKeyShortcuts);
        dispatcher.dispatch<MouseButtonDownEvent>(BIND_EVENT_CB(onMouseButtonDown));
    }

    void EditorLayer::onGUIrender(){
        EV_profile_function();
        // ImGui::ShowDemoWindow();

        MenuPanel::onGUIRender();

        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

        StatsPanel::onGUIrender();
        SceneHeirarchyUI::onGUIrender();
        PropertiesPanel::onGUIrender(SceneHeirarchyUI::getSelectedEntity());
        ScenePanel::onGUIrender(_framebuffer, _camera);
        ContentBrowser::onGUIrender();
        ProjectSettingsUI::onGUIRender(_project);
    }

    bool EditorLayer::onMouseButtonDown(MouseButtonDownEvent& event){
        if(event.getButton() == M_0){
            _framebuffer->bind();
            ScenePanel::mousePickCheck(_framebuffer);
            _framebuffer->unbind();
            return true;
        }
        return false;
    }

    void EditorLayer::loadLayout(){
        std::filesystem::path layoutPath = "layout.ini";
        if(!std::filesystem::exists(layoutPath)) {
            std::filesystem::path defaultLayout = _getEngineAssetsPath("layouts/default.ini");
            if(!std::filesystem::exists(defaultLayout)) return;
            std::filesystem::copy_file(defaultLayout, layoutPath);
        }
        ImGui::LoadIniSettingsFromDisk(layoutPath.string().c_str());
    }

    void EditorLayer::saveLayout(){
        ImGuiIO& io = ImGui::GetIO();
        if(!io.WantSaveIniSettings) return;
        if(!std::filesystem::exists(layoutPath)) return;
        ImGui::SaveIniSettingsToDisk(layoutPath.string().c_str());
    }

    void EditorLayer::loadProject(){
        if(!std::filesystem::exists("Everest.project")) return;
        project_def_t::loadProject("Everest.project", _project);
        
        for(auto& scenep : _project.scene_sequence){
            SceneManager::_instance->sceneSequence = _project.scene_sequence;
            SceneManager::loadScene(_project.loaded_scene.string().c_str());
            _camera.camera = _project.editor_camera_comp;
            _camera.transform = _project.editor_camera_transform;
            ScenePanel::sceneBackgroundColor = _project.background;
        }
    }

    void EditorLayer::saveProject(){
        _project.scene_sequence = SceneManager::_instance->sceneSequence;
        _project.loaded_scene = SceneManager::getSceneTargetPath();
        _project.editor_camera_comp = _camera.camera;
        _project.editor_camera_transform = _camera.transform;
        _project.background = ScenePanel::sceneBackgroundColor;
        project_def_t::saveProject(_project, "Everest.project");
    }
}
