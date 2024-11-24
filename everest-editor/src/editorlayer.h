#pragma once

#include <everest.h>
#include "editor/sceneheirarchy.h"
#include "editor/propspanel.h"

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

    private:
        SpriteSheet _farmsprites;
        ref<Framebuffer> _framebuffer;
        uvec2 _sceneViewPortSize = {1280, 720};
        bool _sceneViewportFocused = false;
    private:
        ref<Scene> _activeScene;
        SceneHeirarchyUI _scenehui;
        PropertiesPanel _props;
        Entity _camera;
};

