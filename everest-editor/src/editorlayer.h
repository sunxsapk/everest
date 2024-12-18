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

        private:
            ref<Framebuffer> _framebuffer;
        private:
            EditorCamera _camera;
    };

}
