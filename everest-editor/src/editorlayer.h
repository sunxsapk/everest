#pragma once

#include <everest.h>

using namespace Everest;


// TODO: write a camera controller for both 2d and 3d scene
class CameraController : public Scriptable {
    public:
        void onCreate(){
            EVLog_Msg("on create camera controller");
        }

        void onUpdate(){
            EVLog_Msg("on update camera controller");
        }

        void onDestroy(){
            EVLog_Msg("on destroy camera controller");
        }
};


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

