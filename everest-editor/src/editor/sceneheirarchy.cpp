#include "sceneheirarchy.h"
#include "scenepanel.h"


namespace Everest {

    Scene* SceneHeirarchyUI::_scene = nullptr;
    Entity SceneHeirarchyUI::_selectedEntity;
    bool SceneHeirarchyUI::_focused = false;

    void SceneHeirarchyUI::setScene(const ref<Scene>& scene){
        _scene = scene.get();
        _selectedEntity = {};
    }

    void SceneHeirarchyUI::onGUIrender(){
        ImGui::Begin("Scene Hierarchy");

        if(_scene != nullptr) {
            static char buf[1<<8];
            memset(buf, 0, sizeof(buf));
            strcpy(buf, _scene->_name.c_str());

            ImGui::AlignTextToFramePadding();
            if(ImGui::InputText("##__scene__", buf, sizeof(buf))){
                _scene->_name = std::string(buf);
            }

            ImGui::Separator();
        }

        heirarchyPopup();
        if(_scene != nullptr){
            for(auto entity: _scene->_registry.view<id_c>()){
                drawEntityNode({entity, _scene});
            }
        }

        if(ImGui::IsWindowHovered() && ImGui::IsMouseDoubleClicked(0)) _selectedEntity = {};
        _focused = ImGui::IsWindowHovered();

        ImGui::End();
    }

    void SceneHeirarchyUI::drawEntityNode(Entity entity){
        if(!entity.isValid()) {
            EVLog_Wrn("Invalid Entity");
            return;
        }

        ImGui::PushID((u32)entity);

        auto& tag = entity.get<tag_c>().tag;
        bool selectAttempt = ImGui::Selectable(tag.c_str(), _selectedEntity == entity);

        bool dragged = false;
        if (ImGui::BeginDragDropSource()) {
            ImGui::SetDragDropPayload("ENTITY_PAYLOAD", &entity, sizeof(entity));
            ImGui::EndDragDropSource();
            dragged = true; 
        }

        if(!dragged && selectAttempt) _selectedEntity = entity;

        bool deleteEntity = false;
        if(ImGui::BeginPopupContextItem()){
            if(ImGui::MenuItem("Delete Entity")) deleteEntity = true;
            if(ImGui::MenuItem("Duplicate Entity")) {
                _scene->duplicateEntity(entity);
            }
            ImGui::EndPopup();
        }

        if(deleteEntity) {
            _scene->destroyEntity(entity);
        }

        ImGui::PopID();
    }

    void SceneHeirarchyUI::heirarchyPopup(){
        if(!_scene) return;
        if(ImGui::BeginPopupContextWindow(0, 1)){
            if(ImGui::MenuItem("Create New Entity")) _scene->createEntity();

            ImGui::EndPopup();
        }
    }

    bool SceneHeirarchyUI::onKeyShortcuts(KeyDownEvent& event){
        if(!_focused) return false;
        if(ScenePanel::getSceneState() != SceneState::EDIT) return false;
        if(!_selectedEntity.isValid()) return false;

        if(event.getKey() == K_delete){
            _scene->destroyEntity(_selectedEntity);
            return true;
        }

        bool isctrl = Input::getKeyDown(K_left_control) || Input::getKeyDown(K_right_control);
        if(!isctrl) return false;

        if(event.getKey() == K_d) _scene->duplicateEntity(_selectedEntity);
        return true;
    }
}
