#include "scenepanel.h"
#include "gizmos.h"


namespace Everest {

    vec2 ScenePanel::_sceneOffset(0);
    uvec2 ScenePanel::_sceneViewPortSize = {1280, 720};
    bool ScenePanel::_focused = false;
    vec4 ScenePanel::sceneBackgroundColor = {.2f, .2f, .2f, 1.f};

    void ScenePanel::onGUIrender(ref<Framebuffer>& sceneRender, EditorCamera& sceneCamera){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Scene");

        sceneSettings(sceneCamera);
        if(sceneViewport(sceneRender)){
            //sceneRender->resize(_sceneViewPortSize); TODO
            sceneCamera.onViewportResize(_sceneViewPortSize);
            auto _activeScene = SceneManager::getActiveScene();
            if(_activeScene) _activeScene->onViewportResize(_sceneViewPortSize);
        }
        Gizmos::renderTransformationGizmo(sceneCamera);

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
        auto off = ImGui::GetItemRectMin();
        _sceneOffset = {off.x, off.y};
        _focused = ImGui::IsWindowFocused();

        return needResize;
    }

    void ScenePanel::sceneSettings(EditorCamera& sceneCamera){
        Camera& cam = sceneCamera.camera;
        ImGui::Spacing();

        bool is2D = cam.getType() == CameraType::Orthographic;
        if(ImGui::RadioButton("2D", is2D)){
            sceneCamera.setType(is2D?Perspective:Orthographic);
        }
        if(is2D) ImGui::SetItemDefaultFocus();

        ImGui::SameLine();
        if(ImGui::Button("Cam Settings")){
            ImGui::OpenPopup("__cam_settings__");
        }

        if(ImGui::BeginPopup("__cam_settings__")){
            f32 sz, nr, fr;
            switch(cam.getType()){
                case CameraType::Orthographic:
                    sz = cam.getOrtho_size(), nr = cam.getOrtho_near(), fr = cam.getOrtho_far();
                    if(ImGui::DragFloat("Ortho Size", &sz, 0.05f)) 
                        cam.setOrtho_size(glm::max(0.f, sz));
                    if(ImGui::DragFloat("Near", &nr, 0.1f)) 
                        cam.setOrtho_near(nr);
                    if(ImGui::DragFloat("Far", &fr, 0.5f)) 
                        cam.setOrtho_far(fr);
                    break;

                case CameraType::Perspective:
                    sz = cam.getPersp_fov(), nr = cam.getPersp_near(), fr = cam.getPersp_far(); 
                    if(ImGui::SliderFloat("FOV", &sz, 0.01f, 180.f)) 
                        cam.setPersp_fov(glm::max(0.f, sz));
                    if(ImGui::DragFloat("Near", &nr, 0.05f)) 
                        cam.setPersp_near(nr);
                    if(ImGui::DragFloat("Far", &fr, 1.f)) 
                        cam.setPersp_far(fr);
                    break;
            }
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
            if(sceneCamera.camera.getType() == Orthographic){
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

}
