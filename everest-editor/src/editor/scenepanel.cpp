#include "scenepanel.h"


namespace Everest {

    uvec2 ScenePanel::_sceneViewPortSize = {1280, 720};

    void ScenePanel::onGUIrender(ref<Framebuffer>& sceneRender, EditorCamera& sceneCamera){
        ImGui::Begin("Scene");

        renderSceneSettings(sceneCamera);
        if(renderSceneViewport(sceneRender)){
            //sceneRender->resize(_sceneViewPortSize); TODO
            sceneCamera.onViewportResize(_sceneViewPortSize);
            auto _activeScene = SceneManager::getActiveScene();
            if(_activeScene) _activeScene->onViewportResize(_sceneViewPortSize);
        }

        ImGui::End();
    }

    bool ScenePanel::renderSceneViewport(ref<Framebuffer>& sceneRender){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        const ImVec2 uv0{0.f, 1.f}, uv1{1.f, 0.f};
        bool _sceneViewportFocused = ImGui::IsWindowFocused();

        ImVec2 _svps_i = ImGui::GetContentRegionAvail();
        uvec2 _svps{_svps_i.x, _svps_i.y};
        bool needResize = false;
        if(_svps != _sceneViewPortSize){
            _sceneViewPortSize = _svps;
            needResize = true;
        }

        ImGui::Image(sceneRender->getColorAttachment(),
                ImVec2(_sceneViewPortSize.x, _sceneViewPortSize.y), uv0, uv1);

        ImGui::PopStyleVar();
        return needResize;
    }

    void ScenePanel::renderSceneSettings(EditorCamera& sceneCamera){
        Camera& cam = sceneCamera.camera;

        bool is2D = cam.getType() == CameraType::Orthographic;
        if(ImGui::RadioButton("2D", is2D)){
            cam.setType(is2D?Perspective:Orthographic);
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
        vec3 fwd = sceneCamera.getForward();
        ImGui::Text("%.2f, %.2f, %.2f", fwd.x, fwd.y, fwd.z);
        ImGui::SameLine();
        if(ImGui::Button("Reset")) sceneCamera.lookAt(vec3(0.f));
    }
}
