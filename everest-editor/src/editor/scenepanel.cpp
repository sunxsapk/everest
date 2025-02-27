#include <filesystem>
#include "scenepanel.h"
#include "gizmos.h"
#include "sceneheirarchy.h"
#include "propspanel.h"


namespace Everest {

    vec2 ScenePanel::_sceneOffset(0);
    uvec2 ScenePanel::_sceneViewPortSize = {1280, 720};
    bool ScenePanel::_focused = false;
    vec4 ScenePanel::sceneBackgroundColor = {.2f, .2f, .2f, 1.f};
    SceneState ScenePanel::_sceneState = SceneState::EDIT;
    ref<Scene> ScenePanel::_runtimeScene = nullptr;
    bool ScenePanel::gameView = true;

    void ScenePanel::onGUIrender(ref<Framebuffer>& sceneRender, EditorCamera& sceneCamera){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene");

        sceneSettings(sceneCamera);
        if(sceneViewport(sceneRender)){
            sceneRender->resize(_sceneViewPortSize);
            sceneCamera.onViewportResize(_sceneViewPortSize);
            auto _activeScene = SceneManager::getActiveScene();
            if(_activeScene) _activeScene->onViewportResize(_sceneViewPortSize);
        }
        if(!gameView || _sceneState == SceneState::EDIT) Gizmos::renderTransformationGizmo(sceneCamera);

        ImGui::End();
        ImGui::PopStyleVar();
    }

    bool ScenePanel::sceneViewport(ref<Framebuffer>& sceneRender){

        const ImVec2 uv0{0.f, 1.f}, uv1{1.f, 0.f};
        ImVec2 _svps_i = ImGui::GetContentRegionAvail();
        uvec2 _svps{_svps_i.x, _svps_i.y};
        bool needResize = false;
        if(_svps != _sceneViewPortSize){
            _sceneViewPortSize = _svps;
            needResize = true;
        }

        ImGui::Image(sceneRender->getColorAttachment(0),
                ImVec2(_sceneViewPortSize.x, _sceneViewPortSize.y), uv0, uv1);

        if(ImGui::BeginDragDropTarget()){
            const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
            if(data != nullptr && data->Data != nullptr){
                const char* path_str = (const char*) data->Data;
                std::filesystem::path path(path_str);
                if(path.extension() == AssetsManager::getScene_ext()){
                    try {
                        if(ScenePanel::getSceneState() != SceneState::EDIT) ScenePanel::onSceneStop();
                        ref<Scene> sc = AssetsManager::loadScene(path);
                        SceneManager::activateScene(sc);
                    } catch(YAML::Exception exc){
                        // TODO: make this into a popup
                        EVLog_Err("Error on loading scene: %s", exc.what());
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }

        auto off = ImGui::GetItemRectMin();
        _sceneOffset = {off.x, off.y};
        _focused = ImGui::IsWindowFocused() || ImGui::IsWindowHovered();

        return needResize;
    }

    bool ScenePanel::onKeyShortcuts(KeyDownEvent& event){
        auto key = event.getKey();

        if(key == K_f5){
            onSceneStateToggle();
            return true;
        }

        return false;
    }

    void ScenePanel::sceneSettings(EditorCamera& sceneCamera){
        Camera& cam = sceneCamera.camera;
        ImGui::Spacing();

        /*ImGui::PushStyleColor(ImGuiCol_Button, {0,0,0,0});
        f32 size = 24.f;
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        spriteRenderer_c& icon = EditorAssets::getIcon(_sceneState == SceneState::EDIT? IconType::PLAY : IconType::STOP);
        if(ImGui::ImageButton("##play", icon.texture->getID(), {size, size}, 
                    {icon.startUV.x, icon.sizeUV.y}, {icon.sizeUV.x, icon.startUV.y}
                )
            ){
            onSceneStateToggle();
        }
        ImGui::PopStyleColor();*/

        const f32 size = 172.f; // magic number from hit n trial
        ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));
        if(ImGui::Button(_sceneState == SceneState::EDIT? "Run" : "Stop")) onSceneStateToggle();
        ImGui::SameLine();
        if(ImGui::Button("Pause")) Time::setPauseState(!Time::isPaused());
        ImGui::SameLine();
        if(ImGui::Button(gameView ? "Game View" : "Scene View")) gameView = !gameView;

        ImVec2 secondRowStart = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(
            secondRowStart,
            ImVec2(secondRowStart.x + ImGui::GetWindowWidth(), secondRowStart.y + 36),
            IM_COL32(32, 32, 32, 255)
        );
        ImGui::Spacing();

        if(ImGui::RadioButton("2D", cam.is2d())){
            if(cam.is2d()) cam.set3d();
            else cam.set2d();
        }
        if(cam.is2d()) ImGui::SetItemDefaultFocus();

        ImGui::SameLine();
        if(ImGui::Button("Cam Settings")){
            ImGui::OpenPopup("__cam_settings__");
        }

        if(ImGui::BeginPopup("__cam_settings__")){
            f32 sz = cam.get_lenssize(),
                asp = cam.get_aspect(),
                nr = cam.get_near(),
                fr = cam.get_far();
            if(PropertiesPanel::_f32dragui(cam.is2d()? "Lens Size" : "FOV", sz, 0.05f, "##size"))
                cam.set_lenssize(glm::max(0.f, sz));
            if(PropertiesPanel::_f32dragui("Aspect Ratio", asp, 0.05f, "##aspect")) 
                cam.set_aspect(glm::max(0.f, asp));
            if(PropertiesPanel::_f32dragui("Near", nr, 0.1f, "##near")) 
                cam.set_near(nr);
            if(PropertiesPanel::_f32dragui("Far", fr, 0.5f, "##far")) 
                cam.set_far(fr);

            ImGui::Separator();
            ImGui::SliderFloat("Speed", &sceneCamera.speed, 0.1f, 100.f, "");
            ImGui::SliderFloat("Mouse Sensitivity", &sceneCamera.mouseSensitivity, 0.01f, 1.f, "");
            ImGui::SliderFloat("Scroll Sensitivity", &sceneCamera.scrollSensitivity, 0.1f, 5.f, "");

            ImGui::Separator();
            ImGui::DragFloat3("Position", glm::value_ptr(sceneCamera.transform.position));
            ImGui::DragFloat3("Rotation", glm::value_ptr(sceneCamera.transform.rotation));

            ImGui::EndPopup();
        }

        ImGui::SameLine();
        if(ImGui::Button("Reset")) {
            if(sceneCamera.camera.is2d()){
                sceneCamera.transform.position.x = 0.f;
                sceneCamera.transform.position.y = 0.f;
            } else sceneCamera.lookAt(vec3(0.f));
        }
        ImGui::SameLine();
        gizmosSettings();
        //vec3 dbgv(Input::mousePosition() - getSceneOffset(), 0.f);
        //ImGui::SameLine();
        //ImGui::Text("%.2f, %.2f, %.2f", dbgv.x, dbgv.y, dbgv.z);
    }

    void ScenePanel::gizmosSettings(){
        if(ImGui::RadioButton("Gizmos", Gizmos::showGizmos)){
            Gizmos::showGizmos = !Gizmos::showGizmos;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Phy-Gizmo", Gizmos::showPhysicsShapes)){
            Gizmos::showPhysicsShapes = !Gizmos::showPhysicsShapes;
        }
        ImGui::SameLine();

        float lineh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec4 cl{sceneBackgroundColor.x, sceneBackgroundColor.y, sceneBackgroundColor.z, sceneBackgroundColor.w};
        ImGui::PushStyleColor(ImGuiCol_Button, cl);
        if(ImGui::Button("Background")) ImGui::OpenPopup("_sc_bg_");
        ImGui::PopStyleColor();

        if(ImGui::BeginPopup("_sc_bg_")){
            ImGui::ColorPicker4("##12", glm::value_ptr(sceneBackgroundColor));
            ImGui::EndPopup();
        }

        bool& lcl = Gizmos::isLocalTransform;
        ImGui::SameLine();
        if(ImGui::Button(lcl?"Local" : "World")) lcl = !lcl;

        ImGuizmo::OPERATION& op = Gizmos::operation;
        ImGui::SameLine();
        if(ImGui::RadioButton("Translate", op == ImGuizmo::TRANSLATE)){
            op = ImGuizmo::TRANSLATE;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Scale", op == ImGuizmo::SCALE)){
            op = ImGuizmo::SCALE;
        }
        ImGui::SameLine();
        if(ImGui::RadioButton("Rotation", op == ImGuizmo::ROTATE)){
            op = ImGuizmo::ROTATE;
        }
    }

    void ScenePanel::mousePickCheck(ref<Framebuffer>& sceneRenderBuffer){
        if(!ImGui::IsMouseClicked(ImGuiMouseButton_Left)) return;
        if(ImGuizmo::IsUsing()) return;
        //if(SceneHeirarchyUI::getSelectedEntity().isValid()) return;

        ivec2 mp = Input::mousePosition() - ScenePanel::getSceneOffset();
        mp.y = ScenePanel::getSceneViewportSize().y - mp.y;
        if(mp.x >= 0 && mp.y >= 0 && mp.x < ScenePanel::getSceneViewportSize().x &&
                mp.y < ScenePanel::getSceneViewportSize().y){
            i32 pixd = sceneRenderBuffer->readPixel(1, mp.x, mp.y);
            if(pixd == -1) SceneHeirarchyUI::_selectedEntity = {};
            else SceneHeirarchyUI::selectEntity((entt::entity)pixd);;
        }
    }

    void ScenePanel::onSceneStop(){
        _sceneState = SceneState::EDIT;
        _runtimeScene->onSceneStop();
        _runtimeScene.reset();
        _runtimeScene = nullptr;
        SceneHeirarchyUI::setScene(getScene());
    }

    void ScenePanel::onScenePlay(){
        _sceneState = SceneState::PLAY;
        ref<Scene> rsc = SceneManager::getActiveScene();
        if(rsc){
            ref<Scene> cp = Scene::copy(rsc);
            _runtimeScene = cp;
            SceneHeirarchyUI::setScene(_runtimeScene);
            _runtimeScene->onScenePlay();
        }
    }
}
