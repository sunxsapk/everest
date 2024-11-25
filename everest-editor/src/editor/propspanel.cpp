#include "propspanel.h"


namespace Everest {
    
    void PropertiesPanel::onGUIrender(Entity ent){
        ImGui::Begin("Properties");
        if(!ent.isValid()){
            ImGui::End();
            return;
        }

        if(ent.has<tag_c>()){
            _tag(ent);
        }


        if(ent.has<transform_c>()){
            _transform(ent);
        }

        if(ent.has<camera_c>()){
            _camera(ent);
        }

        if(ent.has<spriteRenderer_c>()){
            _spriteRenderer(ent);
        }

        if(ent.has<nativeScript_c>()){
            _nativeScript(ent);
        }

        ImGui::End();
    }

    void PropertiesPanel::_tag(Entity& ent){
        auto& tag = ent.get<tag_c>().tag;
        static char buf[1<<8];
        memset(buf, 0, sizeof(buf));
        strcpy(buf, tag.c_str());
        if(ImGui::InputText("Tag", buf, sizeof(buf))){
            tag = std::string(buf);
        }
    }

    void PropertiesPanel::_transform(Entity& ent){
        ImGui::Spacing();
        ImGui::Separator();
        if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)){
            auto& tfr = ent.get<transform_c>().transform ;
            ImGui::DragFloat3("Position", glm::value_ptr(tfr[3]), 0.1f);
            ImGui::TreePop();
        }
    }

    void PropertiesPanel::_camera(Entity& ent){
        ImGui::Spacing();
        ImGui::Separator();
        if(ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)){
            auto& cam_c = ent.get<camera_c>();
            Camera& cam = cam_c.camera;

            CameraType type =  cam.getType();

            {
                const char* projTypes[] = {"Orthographic", "Perspective"};
                const char* curType = projTypes[type];
                if(ImGui::BeginCombo("Projection", curType)){
                    for(i32 i=0; i<2; i++){
                        bool isSel = i == type;
                        if(ImGui::Selectable(projTypes[i], isSel)){
                            curType =  projTypes[i];
                            cam.setType((CameraType)i);
                        }
                        if(isSel) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

            ImGui::Indent();
            if(type == CameraType::Orthographic){
                f32 osz = cam.getOrtho_size(), asp = cam.getOrtho_aspect(),
                    nr = cam.getOrtho_near(), fr = cam.getOrtho_far();

                if(ImGui::DragFloat("Ortho Size", &osz, 0.05f)) 
                    cam.setOrtho_size(glm::max(0.f, osz));
                if(ImGui::DragFloat("Aspect Ratio", &asp, 0.05f)) 
                    cam.setOrtho_aspect(glm::max(0.f, asp));
                if(ImGui::DragFloat("Near", &nr, 0.1f)) 
                    cam.setOrtho_near(nr);
                if(ImGui::DragFloat("Far", &fr, 0.5f)) 
                    cam.setOrtho_far(fr);
            }else{
                f32 fov = cam.getPersp_fov(), asp = cam.getOrtho_aspect(),
                    nr = cam.getPersp_near(), fr = cam.getOrtho_far();

                if(ImGui::DragFloat("FOV", &fov, 0.05f)) 
                    cam.setPersp_fov(glm::max(0.f, fov));
                if(ImGui::DragFloat("Aspect Ratio", &asp, 0.05f)) 
                    cam.setPersp_aspect(glm::max(0.f, asp));
                if(ImGui::DragFloat("Near", &nr, 0.05f)) 
                    cam.setPersp_near(nr);
                if(ImGui::DragFloat("Far", &fr, 1.f)) 
                    cam.setPersp_far(fr);

            }

            ImGui::TreePop();
        }
    }

    void PropertiesPanel::_spriteRenderer(Entity& ent){
    }

    void PropertiesPanel::_nativeScript(Entity& ent){
    }
}
