#include "propspanel.h"
#include "scenepanel.h"


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
            if(ImGui::MenuItem("Camera")) {
                camera_c& cam = ent.tryAdd<camera_c>();
                vec2 viewportSize = ScenePanel::getSceneViewportSize();
                f32 aspect = (float)viewportSize.x / viewportSize.y;
                cam.camera.setOrtho_aspect(aspect);
                cam.camera.setPersp_aspect(aspect);
            }
            if(ImGui::MenuItem("Sprite Renderer")) ent.tryAdd<spriteRenderer_c>();
            if(ImGui::MenuItem("Circle Renderer")) ent.tryAdd<circleRenderer_c>();

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
                if(ImGui::BeginCombo("##proj", curType)){
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

                ImGui::Checkbox("Is Primary", &cam_c.isPrimary);
            }

            ImGui::Separator();

            f32 sz, asp, nr, fr;
            switch(type){
                case CameraType::Orthographic:
                    sz = cam.getOrtho_size(), asp = cam.getOrtho_aspect();
                    nr = cam.getOrtho_near(), fr = cam.getOrtho_far();
                    if(_f32dragui("Ortho Size", sz, 0.05f, "##os"))
                        cam.setOrtho_size(glm::max(0.f, sz));
                    if(_f32dragui("Aspect Ratio", asp, 0.05f, "##oar")) 
                        cam.setOrtho_aspect(glm::max(0.f, asp));
                    ImGui::SameLine();
                    ImGui::Checkbox("Fixed", &cam_c.fixedAspect);
                    if(_f32dragui("Near", nr, 0.1f, "##on")) 
                        cam.setOrtho_near(nr);
                    if(_f32dragui("Far", fr, 0.5f, "##of")) 
                        cam.setOrtho_far(fr);
                    break;

                case CameraType::Perspective:
                    sz = cam.getPersp_fov(), asp = cam.getPersp_aspect();
                    nr = cam.getPersp_near(), fr = cam.getPersp_far(); 
                    if(_f32sliderui("FOV", sz, "##pv", 0.01f, 180.f)) 
                        cam.setPersp_fov(glm::max(0.f, sz));
                    if(_f32dragui("Aspect Ratio", asp, 0.05f, "##par")) 
                        cam.setPersp_aspect(glm::max(0.f, asp));
                    ImGui::SameLine();
                    ImGui::Checkbox("Fixed", &cam_c.fixedAspect);
                    if(_f32dragui("Near", nr, 0.05f, "##pn")) 
                        cam.setPersp_near(nr);
                    if(_f32dragui("Far", fr, 1.f, "##pf")) 
                        cam.setPersp_far(fr);
                    break;
            }
        });

        if(ent.has<spriteRenderer_c>()) _componentUI<spriteRenderer_c>(ent, "Sprite Renderer",
        [](spriteRenderer_c& comp){
            _colorui("Color", comp.color);
            auto& spr = comp.sprite;

            //ImGui::Text("Texture");
            constexpr f32 isize = 64.f;
            //ImGui::SameLine(0.f, 16.f);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton("__texture__", spr.texture? spr.texture->getID() : 1,
                    ImVec2(isize, isize), {spr.startUV.x, spr.sizeUV.y}, {spr.sizeUV.x, spr.startUV.y});
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
                if(data && data->Data){
                    const char* path_str = (const char*) data->Data;
                    if(AssetsManager::getAssetsType(path_str) == AssetsType::TEXTURE){
                        try {
                            spr.texture = AssetsManager::loadTexture(path_str);
                        } catch(std::exception exc){
                            // TODO: make this into a popup
                            EVLog_Err("Error on loading texture: %s", exc.what());
                        }
                    }
                }
                
                ImGui::EndDragDropTarget();
            }
            ImGui::PopStyleColor();
        });

        if(ent.has<circleRenderer_c>()) _componentUI<circleRenderer_c>(ent, "Circle Renderer",
        [](circleRenderer_c& comp){
            _colorui("Color", comp.color);
            _f32sliderui("Thickness", comp.thickness, "##th", 0.f, 1.f);
            _f32sliderui("Fade", comp.fade, "##fd", 0.f, 1.f);
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

    bool PropertiesPanel::_f32dragui(const char* label, f32& value, f32 speed, const char* id){
        const f32 colwidth = 100.f;

        ImGui::PushID(id);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);

        ImGui::NextColumn();

        bool x = ImGui::DragFloat("", &value, speed);
        ImGui::Columns(1);
        ImGui::PopID();
        return x;
    }

    bool PropertiesPanel::_f32sliderui(const char* label, f32& value, const char* id, f32 min_, f32 max_){
        const f32 colwidth = 100.f;

        ImGui::PushID(id);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);

        ImGui::Text("%s", label);
        ImGui::NextColumn();

        bool x = ImGui::SliderFloat("", &value, min_, max_);
        ImGui::Columns(1);
        ImGui::PopID();
        return x;
    }


    void _vec3f32(const char* label, f32 &value, f32 speed, const char* id){
        ImGui::PushFont(UIFontManager::getDefaultBold());
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat(id, &value, speed);
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

        _vec3f32("x", value.x, .1f, "##1");
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vec3f32("y", value.y, .1f, "##2");
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vec3f32("z", value.z, .1f, "##3");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    void PropertiesPanel::_colorui(const char* label, vec4& value, const char* id){
        const f32 colwidth = 100.f;

        ImGui::PushID(id ? id : label);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::NextColumn();
        ImGui::ColorEdit4("##0", glm::value_ptr(value));
        ImGui::Columns(1);
        ImGui::PopID();
    }

}
