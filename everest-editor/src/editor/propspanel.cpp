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
        ImGui::Spacing();
        ImGui::Separator();
    }

    void PropertiesPanel::_transform(Entity& ent){
        if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)){
            auto& tfr = ent.get<transform_c>();

            _vec3ui("Position", tfr.position, 0.f);
            _vec3ui("Rotation", tfr.rotation, 0.f);
            _vec3ui("Scale", tfr.scale, 1.f);

            ImGui::TreePop();
        }
        ImGui::Spacing();
        ImGui::Separator();
    }

    void PropertiesPanel::_camera(Entity& ent){
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

                ImGui::Checkbox("Fixed Aspect Ratio", &cam_c.fixedAspect);
            }

            ImGui::Separator();
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
                f32 fov = cam.getPersp_fov(), asp = cam.getPersp_aspect(),
                    nr = cam.getPersp_near(), fr = cam.getPersp_far();

                if(ImGui::SliderFloat("FOV", &fov, 0.01f, 180.f)) 
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
        ImGui::Spacing();
        ImGui::Separator();
    }

    void PropertiesPanel::_spriteRenderer(Entity& ent){
        if(ImGui::TreeNodeEx("Sprite Renderer", ImGuiTreeNodeFlags_DefaultOpen)){
            auto& sr_c = ent.get<spriteRenderer_c>();
            ImGui::ColorEdit4("Color", glm::value_ptr(sr_c.color));

            auto& spr = sr_c.sprite;
            ImVec2 uv0 = ImVec2(spr.startUV.x, spr.sizeUV.y);
            ImVec2 uv1 = ImVec2(spr.sizeUV.x, spr.startUV.y);
            ImGui::Image(spr.texture->getID(), ImVec2(128,128), uv0, uv1);
            ImGui::TreePop();
        }
        ImGui::Spacing();
        ImGui::Separator();
    }

    void PropertiesPanel::_nativeScript(Entity& ent){
    }

    void PropertiesPanel::_vec3ui(const char* label, vec3& value, f32 resetvalue){
        const f32 colwidth = 100.f;

        ImGui::PushID(label);
        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, colwidth);
        ImGui::Text("%s", label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        f32 lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 btnsz = {lh + 3.f, lh};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.1f, 0.1f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.1f, 1.f));
        if(ImGui::Button("X", btnsz)) value.x = resetvalue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##1", &value.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.1f, 0.6f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.1f, 0.6f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.1f, 0.8f, 1.f));
        if(ImGui::Button("Y", btnsz)) value.y = resetvalue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##2", &value.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.6f, 0.1f, 0.8f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.6f, 0.1f, 1.f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.8f, 0.1f, 1.f));
        if(ImGui::Button("Z", btnsz)) value.z = resetvalue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##3", &value.z, 0.1f);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }
}
