#include "propspanel.h"
#include <filesystem>


namespace Everest {

    const ImGuiTreeNodeFlags PropertiesPanel::_compFlags = ImGuiTreeNodeFlags_DefaultOpen 
        | ImGuiTreeNodeFlags_SpanAvailWidth
        | ImGuiTreeNodeFlags_Framed
        | ImGuiTreeNodeFlags_AllowItemOverlap;

    void PropertiesPanel::onGUIrender(Entity ent){
        ImGui::Begin("Properties");
        if(!ent.isValid()){
            ImGui::End();
            return;
        }

        if(ent.has<tag_c>()) _tag(ent);
        addComponentUI(ent);
        drawComponents(ent);
        ImGui::End();
    }

    void PropertiesPanel::addComponentUI(Entity& ent){
        static const char* lbl = "Add Component";
        f32 txtsz =ImGui::CalcTextSize(lbl).x - GImGui->Style.FramePadding.x;
        ImGui::SameLine(ImGui::GetContentRegionAvail().x - txtsz);

        if(ImGui::Button("Add Component")){
            ImGui::OpenPopup("_add_component_");
        }

        if(ImGui::BeginPopup("_add_component_")){
            if(ImGui::MenuItem("Camera")) ent.add<camera_c>();
            if(ImGui::MenuItem("Sprite Renderer")) ent.add<spriteRenderer_c>();

            ImGui::EndPopup();
        }
    }

    template<typename comp_t, typename uiDrawCallback>
    void PropertiesPanel::_componentUI(Entity& ent, const char* label, uiDrawCallback callback){
        ImVec2 craw = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});
        float lineh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImGui::Separator();

        bool c_remove = false;
        bool c_open = ImGui::TreeNodeEx(label, _compFlags);
        ImGui::PopStyleVar();
        ImGui::SameLine(craw.x - lineh * 0.5f);

        ImGui::PushFont(UIFontManager::getDefaultBold());
        if(ImGui::Button("+", ImVec2{lineh, lineh})) ImGui::OpenPopup("_comp_settings_");
        ImGui::PopFont();

        if(ImGui::BeginPopup("_comp_settings_")){
            if(ImGui::MenuItem("Remove component")) c_remove = true;
            ImGui::EndPopup();
        }

        if(c_open){
            auto& comp = ent.get<comp_t>();
            callback(comp);
            ImGui::TreePop();
        }
        ImGui::Spacing();
        if(c_remove) ent.remove<comp_t>();
    }

    void PropertiesPanel::drawComponents(Entity& ent){
        if(ent.has<transform_c>()) _transform(ent);

        if(ent.has<camera_c>()) _componentUI<camera_c>(ent, "Camera",
        [](camera_c& cam_c) {
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

            f32 sz, asp, nr, fr;
            switch(type){
                case CameraType::Orthographic:
                    sz = cam.getOrtho_size(), asp = cam.getOrtho_aspect();
                    nr = cam.getOrtho_near(), fr = cam.getOrtho_far();
                    if(ImGui::DragFloat("Ortho Size", &sz, 0.05f)) 
                        cam.setOrtho_size(glm::max(0.f, sz));
                    if(ImGui::DragFloat("Aspect Ratio", &asp, 0.05f)) 
                        cam.setOrtho_aspect(glm::max(0.f, asp));
                    if(ImGui::DragFloat("Near", &nr, 0.1f)) 
                        cam.setOrtho_near(nr);
                    if(ImGui::DragFloat("Far", &fr, 0.5f)) 
                        cam.setOrtho_far(fr);
                    break;

                case CameraType::Perspective:
                    sz = cam.getPersp_fov(), asp = cam.getPersp_aspect();
                    nr = cam.getPersp_near(), fr = cam.getPersp_far(); 
                    if(ImGui::SliderFloat("FOV", &sz, 0.01f, 180.f)) 
                        cam.setPersp_fov(glm::max(0.f, sz));
                    if(ImGui::DragFloat("Aspect Ratio", &asp, 0.05f)) 
                        cam.setPersp_aspect(glm::max(0.f, asp));
                    if(ImGui::DragFloat("Near", &nr, 0.05f)) 
                        cam.setPersp_near(nr);
                    if(ImGui::DragFloat("Far", &fr, 1.f)) 
                        cam.setPersp_far(fr);
                    break;
            }
        });

        if(ent.has<spriteRenderer_c>()) _componentUI<spriteRenderer_c>(ent, "Sprite Renderer",
        [](spriteRenderer_c& comp){
            _colorui("Color", comp.color);
            auto& spr = comp.sprite;

            ImGui::Text("Texture");

            constexpr f32 isize = 32.f;
            ImGui::SameLine(0.f, 16.f);

            ImGui::ImageButton("__texture__", spr.texture? spr.texture->getID() : 1,
                    ImVec2(isize, isize), {spr.startUV.x, spr.sizeUV.y}, {spr.sizeUV.x, spr.startUV.y});
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
                if(data && data->Data){
                    const char* path_str = (const char*) data->Data;
                    try {
                        spr.texture = createRef<Texture>(path_str); // TODO: make an assets manager that fetched preloaded assets from path
                    } catch(std::exception exc){
                        // TODO: make this into a popup
                        EVLog_Err("Error on loading texture: %s", exc.what());
                    }
                }
                
                ImGui::EndDragDropTarget();
            }
        });

        if(ent.has<nativeScript_c>()) _componentUI<nativeScript_c>(ent, "Native Script",
        [](nativeScript_c& comp){
        });
    }

    void PropertiesPanel::_tag(Entity& ent){
        auto& tag = ent.get<tag_c>().tag;
        static char buf[1<<8];
        memset(buf, 0, sizeof(buf));
        strcpy(buf, tag.c_str());

        ImGui::AlignTextToFramePadding();
        ImGui::Text("Tag");
        ImGui::SameLine();
        if(ImGui::InputText("##xx", buf, sizeof(buf))){
            tag = std::string(buf);
        }
    }

    void PropertiesPanel::_transform(Entity& ent){
        if(ImGui::TreeNodeEx("Transform", _compFlags)){
            auto& tfr = ent.get<transform_c>();

            _vec3ui("Position", tfr.position, 0.f);
            _vec3ui("Rotation", tfr.rotation, 0.f);
            _vec3ui("Scale", tfr.scale, 1.f);

            ImGui::TreePop();
        }
        ImGui::Spacing();
    }

    void PropertiesPanel::_f32dragui(const char* label, f32& value, ImVec2 size, const char* id){
        ImGui::PushFont(UIFontManager::getDefaultBold());
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::PopFont();

        ImGui::SameLine(0.f, 2.f);
        ImGui::DragFloat(id, &value, 0.1f);
        ImGui::PopItemWidth();
    }

    void PropertiesPanel::_f32sliderui(const char* label, f32& value, const char* id){
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::SliderFloat(id, &value, 0.01f, 180.f);
    }

    void PropertiesPanel::_vec3ui(const char* label, vec3& value, f32 resetvalue){
        const f32 colwidth = 100.f;

        ImGui::PushID(label);
        ImGui::Columns(2);

        ImGui::AlignTextToFramePadding();
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::Text("%s", label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        f32 lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 btnsz = {lh + 3.f, lh};

        _f32dragui("x", value.x, btnsz, "##1");
        ImGui::SameLine(0.f, 4.f);
        _f32dragui("y", value.y, btnsz, "##2");
        ImGui::SameLine(0.f, 4.f);
        _f32dragui("z", value.z, btnsz, "##3");

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    void PropertiesPanel::_colorui(const char* label, vec4& value){
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::SameLine();
        ImGui::ColorEdit4("##0", glm::value_ptr(value));
    }

}
