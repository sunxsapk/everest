#pragma once


#include <everest.h>


namespace Everest {

    class SceneHeirarchyUI {
        public:
            static void setScene(const ref<Scene>& scene);

            static void onGUIrender();

            static inline Entity getSelectedEntity(){return _selectedEntity;}

        private:
            static void drawEntityNode(Entity entity);
            static void heirarchyPopup();
            static inline void selectEntity(entt::entity id){
                _selectedEntity = {id, _scene};}

        private:
            static Scene* _scene;
            static Entity _selectedEntity;
            friend class PropertiesPanel;
            friend class ScenePanel;
    };

}
