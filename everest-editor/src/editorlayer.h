#pragma once

#include <everest.h>

using namespace Everest;

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
        void handleMenuBar();

    private:
        SpriteSheet _farmsprites;
        ref<Framebuffer> _framebuffer;
        uvec2 _sceneViewPortSize = {1280, 720};
        bool _sceneViewportFocused = false;
    private:
        Scene _activeScene;
        Entity _camera;
};

