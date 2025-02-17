#include "sceneheirarchy.h"
#include "scenepanel.h"


namespace Everest {

    Scene* SceneHeirarchyUI::_scene = nullptr;
    Entity SceneHeirarchyUI::_selectedEntity;

    void SceneHeirarchyUI::setScene(const ref<Scene>& scene){
        _scene = scene.get();
        _selectedEntity = {};
    }

    void SceneHeirarchyUI::onGUIrender(){
        ImGui::Begin("Scene Hierarchy");

        heirarchyPopup();
        if(_scene != nullptr){
            for(auto entity: _scene->_registry.view<tag_c>()){
                drawEntityNode({entity, _scene});
            }
        }

        if(ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) _selectedEntity = {};

        ImGui::End();
    }

    void SceneHeirarchyUI::drawEntityNode(Entity entity){
        auto& tag = entity.get<tag_c>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow 
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | (_selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity, flags, "%s", tag.c_str());
        if(ImGui::IsItemClicked()) _selectedEntity = entity;

        bool deleteEntity = false;
        if(ImGui::BeginPopupContextItem()){
            if(ImGui::MenuItem("Delete Entity")) deleteEntity = true;
            if(ImGui::MenuItem("Duplicate Entity")) {
                _scene->duplicateEntity(entity);
            }
            ImGui::EndPopup();
        }

        if(opened){
            ImGui::TreePop();
        }

        if(deleteEntity) {
            _scene->destroyEntity(entity);
        }
    }

    void SceneHeirarchyUI::heirarchyPopup(){
        if(!_scene) return;
        if(ImGui::BeginPopupContextWindow(0, 1)){
            if(ImGui::MenuItem("Create New Entity")) _scene->createEntity();

            ImGui::EndPopup();
        }
    }

    bool SceneHeirarchyUI::onKeyShortcuts(KeyDownEvent& event){
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
