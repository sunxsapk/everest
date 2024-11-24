#include "sceneheirarchy.h"


namespace Everest {

    SceneHeirarchyUI::SceneHeirarchyUI(const ref<Scene>& scene) {
        setScene(scene);
    }

    void SceneHeirarchyUI::setScene(const ref<Scene>& scene){
        _scene = scene;
    }

    void SceneHeirarchyUI::onGUIrender(){
        ImGui::Begin("Scene Hierarchy");

        for(auto entity: _scene->_registry.view<tag_c>()){
            drawEntityNode({entity, _scene.get()});
        }

        if(ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) _selectedEntity = {};

        ImGui::End();
    }

    void SceneHeirarchyUI::drawEntityNode(Entity entity){
        auto& tag = entity.get<tag_c>().tag;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow
            | (_selectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0);
        bool opened = ImGui::TreeNodeEx((void*)(u64)(u32)entity, flags, "%s", tag.c_str());
        if(ImGui::IsItemClicked()) _selectedEntity = entity;

        if(opened){
            ImGui::TreePop();
        }
    }
}
