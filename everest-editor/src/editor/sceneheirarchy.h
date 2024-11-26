#pragma once


#include <everest.h>


namespace Everest {

    class SceneHeirarchyUI {
        public:
            SceneHeirarchyUI() = default;
            SceneHeirarchyUI(const ref<Scene>& scene);

            void setScene(const ref<Scene>& scene);

            void onGUIrender();

            inline Entity getSelectedEntity(){return _selectedEntity;}
        private:
            void drawEntityNode(Entity entity);
            void heirarchyPopup();

        private:
            Entity _selectedEntity;
            friend class PropertiesPanel;
        private:
            ref<Scene> _scene;
    };
}
