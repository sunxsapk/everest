#pragma once

#include <everest.h>
#include "editor/editorcamera.h"

namespace Everest {

    class EditorLayer : public Layer {
        public:
            EditorLayer(const char* name);
            void onAttach() override;
            void onUpdate() override;
            void onGUIrender() override;
            void onEvent(Event& event) override;
            void onDetach() override;
        private:
            void handleSceneViewPort();
            bool onMouseButtonDown(MouseButtonDownEvent& event);

            void editorView(ref<Scene>& activeScene, bool update = false);

            void loadLayout();
            void saveLayout();

            void loadProject();
            void saveProject();

        private:
            ref<Framebuffer> _framebuffer;
        private:
            EditorCamera _camera;
            std::filesystem::path layoutPath;
            project_def_t _project;
    };

}
