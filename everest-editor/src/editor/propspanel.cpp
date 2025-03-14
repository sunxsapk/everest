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
        drawScripts(ent);
        scriptDropper(ent);

        ImGui::End();
    }

    void PropertiesPanel::scriptDropper(Entity& ent){
        ImVec2 region = ImGui::GetContentRegionAvail();

        ImGui::Button("Drop your script here", ImVec2(region.x, 150));

        if(ImGui::BeginDragDropTarget()){
            const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
            if(data && data->Data){
                const char* path_str = (const char*) data->Data;
                EVLog_Msg("Drag n Drop Item %s", path_str);
                if(AssetsManager::getAssetsType(path_str) == AssetsType::SCRIPT){
                    try {
                        auto& scr = ent.tryAdd<EvScript>();
                        scr.addScript(path_str);
                    } catch(std::exception exc){
                        // TODO: make this into a popup
                        EVLog_Err("Error on loading Script: %s", exc.what());
                    }
                }
            }
            ImGui::EndDragDropTarget();
        }
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
                cam.set_aspect(aspect);
            }
            if(ImGui::MenuItem("Sprite Renderer")) ent.tryAdd<spriteRenderer_c>();
            if(ImGui::MenuItem("Circle Renderer")) ent.tryAdd<circleRenderer_c>();
            if(ImGui::MenuItem("Rigidbody 2D")) ent.tryAdd<rigidbody2d_c>();
/*#ifndef __NO_3D__
            if(ImGui::MenuItem("Rigidbody")) ent.tryAdd<rigidbody_c>();
            if(ImGui::MenuItem("Spring Joint")) ent.tryAdd<springJoint_c>();
#endif*/
            if(ImGui::MenuItem("Spring Joint 2D")) ent.tryAdd<springJoint2d_c>();
            if(ImGui::MenuItem("Box Collider 2D")) ent.tryAdd<boxCollider2d_c>();
            if(ImGui::MenuItem("Circle Collider 2D")) ent.tryAdd<circleCollider2d_c>();
            // if(ImGui::MenuItem("Everest Script")) ent.tryAdd<EvScript>(ent); // Not needed for now

            ImGui::EndPopup();
        }
    }

    bool PropertiesPanel::_scriptWrapper(Scripting::scriptHandler_t& script, Entity ent){
        ImGui::PushID(&script);
        ImVec2 craw = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});
        float lineh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

        bool c_open = ImGui::TreeNodeEx("##_scr__", _compFlags,
                "%s", script.scriptpath.empty() ? "-- None --" : script.getScriptName().c_str()
            );
        ImGui::PopStyleVar();
        ImGui::SameLine(craw.x - lineh * 0.5f);

        ImGui::PushFont(UIFontManager::getDefaultBold());
        if(ImGui::Button("+", ImVec2{lineh, lineh})) ImGui::OpenPopup("__scr_st__");
        ImGui::PopFont();

        bool removeReq = false;
        if(ImGui::BeginPopup("__scr_st__")){
            removeReq = ImGui::MenuItem("Remove");

            if(ImGui::MenuItem("Reload")){
                try {
                    script.init(ent);
                } catch(std::exception exc){
                    EVLog_Err("Error on reloading Script: %s", exc.what());
                }
            }
            ImGui::EndPopup();
        }

        if(c_open){
            _serializeFields(script);
            ImGui::TreePop();
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::PopID();
        return removeReq;
    }

    void PropertiesPanel::drawScripts(Entity& ent){
        if(!ent.has<EvScript>()) return;
        auto& script = ent.get<EvScript>();

        int remove = -1;
        for(int i = 0; i < script.scripts.size(); i++){
            auto& scr = script.scripts[i];
            if(_scriptWrapper(scr, ent)){
                remove = i;
            }
        }
        if(remove >= 0) script.scripts.erase(script.scripts.begin() + remove);
    }

    template<typename comp_t, typename uiDrawCallback>
    void PropertiesPanel::_componentUI(Entity& ent, const char* label, uiDrawCallback callback){
        ImVec2 craw = ImGui::GetContentRegionAvail();
        auto& comp = ent.get<comp_t>();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});
        float lineh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;

        bool c_remove = false;
        bool c_open = ImGui::TreeNodeEx(label, _compFlags);
        ImGui::PopStyleVar();
        ImGui::SameLine(craw.x - lineh - GImGui->Style.FramePadding.x * 2.f - GImGui->Style.ItemSpacing.x);

        ImGui::PushFont(UIFontManager::getDefaultBold());
        ImGui::Checkbox("##_a", &comp.active);
        ImGui::SameLine();
        if(ImGui::Button("+", ImVec2{lineh, lineh})) ImGui::OpenPopup("_comp_settings_");
        ImGui::PopFont();

        if(ImGui::BeginPopup("_comp_settings_")){
            if(ImGui::MenuItem("Remove component")) c_remove = true;
            ImGui::EndPopup();
        }

        if(c_open){
            callback(comp);
            ImGui::TreePop();
        }
        ImGui::Spacing();
        ImGui::Separator();
        if(c_remove) ent.remove<comp_t>();
    }

    void PropertiesPanel::_serializeFields(Scripting::scriptHandler_t& script){
        using namespace Scripting;

        sol::table fields;
        if(!script.getSerializedFields(fields)) return;

        for(auto& [k, v] : fields){
            const char* key = k.as<const char*>();
            auto& scrstate = *script.state;
            Types type = v.as<Types>();

            if(type != Types::Entity && !scrstate[key].valid()) continue;

            ImGui::PushID(key);


            switch(type){ // TODO: better ui
                case Types::Bool: {
                    bool val = scrstate[key];
                    if(_boolui(key, val, key)) scrstate[key] = val;
                    break;
                }

                case Types::Int: {
                    int val = scrstate[key];
                    if(_i32ui(key, val)) scrstate[key] = val;
                    break;
                }

                case Types::Float: {
                    f32 val = scrstate[key];
                    if(_f32dragui(key, val, 0.05f, key)) scrstate[key] = val;
                    break;
                }

                case Types::String: {
                    std::string val = scrstate[key];
                    static char buffer[1<<8];
                    memset(buffer, 0, sizeof(buffer));
                    strcpy(buffer, val.c_str());
                    ImGui::Columns(2);
                    ImGui::SetColumnWidth(0, colwidth);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("%s", key);
                    ImGui::NextColumn();
                    if(ImGui::InputText("##xx", buffer, sizeof(buffer))){
                        scrstate[key] = std::string(buffer);
                    }
                    ImGui::Columns(1);
                    break;
                }

                case Types::Vec2: {
                    vec2& val = scrstate[key];
                    _vec2ui(key, val, 0.f);
                    break;
                }

                case Types::Vec3: {
                    vec3& val = scrstate[key];
                    _vec3ui(key, val, 0.f);
                    break;
                }

                case Types::Vec4: {
                    vec4& val = scrstate[key];
                    _vec4ui(key, val, 0.f);
                    break;
                }

                case Types::Color: {
                    vec4& val = scrstate[key];
                    _colorui(key, val);
                    break;
                }

                case Types::Entity: {
                    Entity _e;
                    if(scrstate[key].valid()) _e = scrstate[key];
                    if(_entity(key, _e)){
                        scrstate[key] = _e;
                    }
                    break;
                }

                default:
                    ImGui::TextColored(ImVec4(.8f, 0.f, 0.f, 1.f), "Unsupported Type");
                    break;
            }
            ImGui::PopID();
        }
    }

    bool PropertiesPanel::_entity(const char* label, Entity& entity){
        bool used = false;
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label);
        ImGui::NextColumn();
        ImGui::Button(entity.isValid()? entity.get<tag_c>().tag.c_str() : "-- None --");

        if(ImGui::BeginDragDropTarget()){
            const ImGuiPayload* data = ImGui::AcceptDragDropPayload("ENTITY_PAYLOAD");
            if(data && data->Data){
                entity = *(Entity*)data->Data;
                used = true;
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::Columns(1);
        return used;
    }

    void PropertiesPanel::drawComponents(Entity& ent){
        if(ent.has<transform_c>()) _transform(ent);

        if(ent.has<camera_c>()) _componentUI<camera_c>(ent, "camera_c",
        [](camera_c& cam) {
            {
                const char* projTypes[] = {"Orthographic", "Perspective"};
                const char* curType = projTypes[cam.is3d()];
                if(ImGui::BeginCombo("##proj", curType)){
                    if(ImGui::Selectable(projTypes[0], cam.is2d())) cam.set2d();
                    if(cam.is2d()) ImGui::SetItemDefaultFocus();

                    if(ImGui::Selectable(projTypes[1], cam.is3d())) cam.set3d();
                    if(cam.is3d()) ImGui::SetItemDefaultFocus();

                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Is Primary", &cam.isPrimary);
            }

            ImGui::Separator();

            f32 sz = cam.get_lenssize(),
                asp = cam.get_aspect(),
                nr = cam.get_near(),
                fr = cam.get_far();

            if(_f32dragui(cam.is2d()? "Lens Size" : "FOV", sz, 0.05f, "##size"))
                cam.set_lenssize(glm::max(0.f, sz));
            if(_f32dragui("Aspect Ratio", asp, 0.05f, "##aspect")) 
                cam.set_aspect(glm::max(0.f, asp));
            ImGui::SameLine();
            ImGui::Checkbox("Fixed", &cam.fixedAspect);
            if(_f32dragui("Near", nr, 0.1f, "##near", -std::numeric_limits<f32>::max(), fr))
                cam.set_near(nr);
            if(_f32dragui("Far", fr, 0.5f, "##far", nr, std::numeric_limits<f32>::max())) 
                cam.set_far(fr);
        });

        if(ent.has<spriteRenderer_c>()) _componentUI<spriteRenderer_c>(ent, "Sprite Renderer",
        [](spriteRenderer_c& spr){
            _colorui("Color", spr.color);
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
            if(ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()){
                spr.texture = nullptr;
            }
            ImGui::PopStyleColor();
        });

        if(ent.has<circleRenderer_c>()) _componentUI<circleRenderer_c>(ent, "Circle Renderer",
        [](circleRenderer_c& comp){
            _colorui("Color", comp.color);
            _f32sliderui("Thickness", comp.thickness, "##th", 0.f, 1.f);
            _f32sliderui("Fade", comp.fade, "##fd", 0.f, 1.f);

            constexpr f32 isize = 64.f;

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
            ImGui::ImageButton("__texture__", comp.texture? comp.texture->getID() : 1,
                    ImVec2(isize, isize), {comp.startUV.x, comp.sizeUV.y}, {comp.sizeUV.x, comp.startUV.y});
            if(ImGui::BeginDragDropTarget()){
                const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
                if(data && data->Data){
                    const char* path_str = (const char*) data->Data;
                    if(AssetsManager::getAssetsType(path_str) == AssetsType::TEXTURE){
                        try {
                            comp.texture = AssetsManager::loadTexture(path_str);
                        } catch(std::exception exc){
                            // TODO: make this into a popup
                            EVLog_Err("Error on loading texture: %s", exc.what());
                        }
                    }
                }
                
                ImGui::EndDragDropTarget();
            }
            if(ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered()){
                comp.texture = nullptr;
            }
            ImGui::PopStyleColor();
        });

/*#ifndef __NO_3D__
        if(ent.has<rigidbody_c>()) _componentUI<rigidbody_c>(ent, "Rigidbody",
        [](rigidbody_c& comp){
            f32 mass = comp.getMass();
            f32 drag = comp.drag;
            _vec3ui("Velocity", comp.velocity, 0.f);
            _vec3ui("Angular Velocity", comp.angularVelocity, 0.f);
            if(_f32dragui("Mass", mass, 0.01f, "##mass",
                        std::numeric_limits<f32>::min(),
                        std::numeric_limits<f32>::max())) comp.setMass(mass);
            _f32dragui("Inverse Inertia", comp.inverseInertia, 0.01f, "#invin");
            if(_f32dragui("Drag", drag, 0.01f, "##drag") && drag >= 0.f) comp.drag = drag;
            ImGui::Checkbox("Use Gravity", &comp.useGravity);
        });
#endif*/

        if(ent.has<rigidbody2d_c>()) _componentUI<rigidbody2d_c>(ent, "Rigidbody 2D",
        [](rigidbody2d_c& comp){
            f32 mass = comp.getMass();
            f32 drag = comp.drag;
            bool ug = comp.definition&BodyDefBits::UseGravity, st = comp.definition&Static;

            if(ImGui::Checkbox("Static", &st)) comp.definition ^= Static;
            if(st) return;

            _vec2ui("Velocity", comp.velocity, 0.f);
            _f32dragui("Angular Velocity", comp.angularVelocity, 0.01f, "##angv");
            if(_f32dragui("Mass", mass, 0.01f, "##mass",
                        std::numeric_limits<f32>::min(),
                        std::numeric_limits<f32>::max())) comp.setMass(mass);
            _f32dragui("Inverse Inertia", comp.inverseInertia, 0.01f, "#invin", std::numeric_limits<f32>::min(), std::numeric_limits<f32>::max());
            if(_f32dragui("Drag", drag, 0.01f, "##drag", 0.f, std::numeric_limits<f32>::max())) comp.drag = drag;
            if(ImGui::Checkbox("Use Gravity", &ug)) comp.definition ^= UseGravity;
        });

        if(ent.has<springJoint2d_c>()) _componentUI<springJoint2d_c>(ent, "Spring Joint 2D",
        [](springJoint2d_c& comp){
            //_vec2ui("Anchor", comp.anchor, 0.f);
            _entity("Anchor", comp.anchor);
            _vec2ui("Offset", comp.offset, 0.f);
            _f32dragui("Spring Constant", comp.springConstant, 0.01f, "##spk") ;
            _f32dragui("Damping", comp.damping, 0.01f, "##dmp", 0.f, std::numeric_limits<f32>::max());
            _f32dragui("Rest Length", comp.restLength, 0.01f, "##rl");
        });

/*#ifndef __NO_3D__
        if(ent.has<springJoint_c>()) _componentUI<springJoint_c>(ent, "Spring Joint",
        [](springJoint_c& comp){
            _vec3ui("Anchor", comp.anchor, 0.f);
            _vec3ui("Offset", comp.offset, 0.f);
            _f32dragui("Spring Constant", comp.springConstant, 0.01f, "##spk") ;
            _f32dragui("Damping", comp.damping, 0.01f, "##dmp") ;
            _f32dragui("Rest Length", comp.restLength, 0.01f, "##rl");
        });
#endif*/

        if(ent.has<boxCollider2d_c>()) _componentUI<boxCollider2d_c>(ent, "Box Collider 2D",
        [](boxCollider2d_c& comp){
            _vec2ui("Offset", comp.box.offset, 0.f);
            _vec2ui("Half Extents", comp.box.halfExtents, 0.5f, 0.f, std::numeric_limits<f32>::max());
            _f32sliderui("Restitution", comp.restitution, "##brst");
        });

        if(ent.has<circleCollider2d_c>()) _componentUI<circleCollider2d_c>(ent, "Circle Collider 2D",
        [](circleCollider2d_c& comp){
            _vec2ui("Offset", comp.circle.offset, 0.f);
            _f32dragui("Radius", comp.circle.radius, 0.5f);
            _f32sliderui("Restitution", comp.restitution, "##crst");
        });

#if 0
        if(ent.has<nativeScript_c>()) _componentUI<nativeScript_c>(ent, "Native Script",
        [](nativeScript_c& comp){
        });
#endif

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
            _vec3ui("Scale", tfr.scale, 1.f, 0.f, std::numeric_limits<f32>::max());

            ImGui::TreePop();
        }
        ImGui::Spacing();
        ImGui::Separator();
    }

    bool PropertiesPanel::_boolui(const char* label, bool& value, const char* id){
        ImGui::PushID(id);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);

        ImGui::NextColumn();

        bool x = ImGui::Checkbox("", &value);
        ImGui::Columns(1);
        ImGui::PopID();
        return x;
    }

    bool PropertiesPanel::_i32ui(const char* label, i32& value, const char* id){
        ImGui::PushID(id);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);

        ImGui::NextColumn();

        bool x = ImGui::DragInt("", &value);
        ImGui::Columns(1);
        ImGui::PopID();
        return x;
    }

    bool PropertiesPanel::_f32dragui(const char* label, f32& value, f32 speed, const char* id, f32 min, f32 max){
        ImGui::PushID(id);
        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, colwidth);

        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);

        ImGui::NextColumn();

        bool x = ImGui::DragFloat("", &value, speed, min, max);
        ImGui::Columns(1);
        ImGui::PopID();
        return x;
    }

    bool PropertiesPanel::_f32sliderui(const char* label, f32& value, const char* id, f32 min_, f32 max_){
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


    void _vecxf32ui(const char* label, f32 &value, f32 speed, const char* id, f32 min = 0.f, f32 max = 0.f){
        ImGui::PushFont(UIFontManager::getDefaultBold());
        ImGui::AlignTextToFramePadding();
        ImGui::Text("%s", label);
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::DragFloat(id, &value, speed, min, max);
    }

    void PropertiesPanel::_vec2ui(const char* label, vec2& value, f32 resetvalue, f32 min, f32 max){
        ImGui::PushID(label);
        ImGui::Columns(2);

        ImGui::AlignTextToFramePadding();
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::Text("%s", label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        f32 lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 btnsz = {lh + 3.f, lh};

        _vecxf32ui("x", value.x, .1f, "##1", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("y", value.y, .1f, "##2", min, max);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    void PropertiesPanel::_vec3ui(const char* label, vec3& value, f32 resetvalue, f32 min, f32 max){
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

        _vecxf32ui("x", value.x, .1f, "##1", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("y", value.y, .1f, "##2", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("z", value.z, .1f, "##3", min, max);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    void PropertiesPanel::_vec4ui(const char* label, vec4& value, f32 resetvalue, f32 min, f32 max){
        ImGui::PushID(label);
        ImGui::Columns(2);

        ImGui::AlignTextToFramePadding();
        ImGui::SetColumnWidth(0, colwidth);
        ImGui::Text("%s", label);
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0,0});

        f32 lh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 btnsz = {lh + 3.f, lh};

        _vecxf32ui("x", value.x, .1f, "##1", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("y", value.y, .1f, "##2", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("z", value.z, .1f, "##3", min, max);
        ImGui::PopItemWidth();
        ImGui::SameLine(0.f, 4.f);
        _vecxf32ui("w", value.w, .1f, "##4", min, max);
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);

        ImGui::PopID();
    }

    void PropertiesPanel::_colorui(const char* label, vec4& value, const char* id){
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



#if 0
    bool PropertiesPanel::_scriptHandler(Scripting::scriptHandler_t& script, Entity ent){
        const char* sn = script.scriptpath.empty()? "None" : script.scriptpath.c_str();
        auto style = ImGui::GetStyle();
        f32 height = UIFontManager::getDefaultBold()->FontSize;
        f32 width = ImGui::GetContentRegionAvail().x;
        float lineh = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImVec2 beg = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(beg, {beg.x + width - style.WindowPadding.x, beg.y + height + 2 * style.FramePadding.y},
                ImColor(.4f, .4f, .4f, .5f));

        beg.x += style.FramePadding.x;
        beg.y += style.FramePadding.y;
        ImGui::SetCursorScreenPos(beg);
        ImGui::InvisibleButton("##dndtrg", {width - style.WindowPadding.x - lineh * 3, height});

        ImGui::SetItemAllowOverlap();

        if(ImGui::BeginDragDropTarget()){
            const ImGuiPayload* data = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
            if(data && data->Data){
                const char* path_str = (const char*) data->Data;
                EVLog_Msg("Drag n Drop Item %s", path_str);
                if(AssetsManager::getAssetsType(path_str) == AssetsType::SCRIPT){
                    try {
                        script.setScriptPath(std::filesystem::path(path_str), ent);
                    } catch(std::exception exc){
                        // TODO: make this into a popup
                        EVLog_Err("Error on loading Script: %s", exc.what());
                    }
                }
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::SetCursorScreenPos(beg);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.f, 1.f, 1.f));

        ImGui::Text("[ %s ] >> %s", script.scriptpath.empty() ? "--" : script.getScriptName().c_str(), sn);

        beg.x += width - lineh * 3 - style.FramePadding.x;
        beg.y -= style.FramePadding.y;
        ImGui::SetCursorScreenPos(beg);
        bool clearReq = ImGui::Button("x", ImVec2{lineh, lineh});
        ImGui::SameLine();
        if(ImGui::Button("r", ImVec2{lineh, lineh})){
            script.init(ent);
        }

        ImGui::PopStyleColor();

        _serializeFields(script);

        return clearReq;
    }
#endif

}
