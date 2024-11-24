#include "propspanel.h"


namespace Everest {
    
    void PropertiesPanel::onGUIrender(Entity ent){
        ImGui::Begin("Properties");
        if(!ent.isValid()){
            ImGui::End();
            return;
        }

        if(ent.has<tag_c>()){
            auto& tag = ent.get<tag_c>().tag;
            static char buf[1<<8];
            memset(buf, 0, sizeof(buf));
            strcpy(buf, tag.c_str());
            if(ImGui::InputText("Tag", buf, sizeof(buf))){
                tag = std::string(buf);
            }
        }


        if(ent.has<transform_c>()){
            ImGui::Spacing();
            if(ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen)){
                auto& tfr = ent.get<transform_c>().transform ;
                ImGui::DragFloat3("Position", glm::value_ptr(tfr[3]), 0.1f);
                ImGui::TreePop();
            }
        }

        if(ent.has<camera_c>()){
            ImGui::Spacing();
            if(ImGui::TreeNodeEx("Camera", ImGuiTreeNodeFlags_DefaultOpen)){
                auto& cam = ent.get<camera_c>();

                CameraType type =  cam.camera.getType();

                {
                    const char* projTypes[] = {"Perspective", "Orthographic"};
                    const char* curType = projTypes[type];
                    if(ImGui::BeginCombo("Projection", curType)){
                        for(i32 i=0; i<2; i++){
                            bool isSel = i == type;
                            if(ImGui::Selectable(projTypes[i], isSel)){
                                curType =  projTypes[i];
                                cam.camera.setType((CameraType)i);
                            }
                            if(isSel) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                }

                ImGui::TreePop();
            }
        }

        ImGui::End();
    }
}
