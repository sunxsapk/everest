#pragma once

#include <everest.h>

using namespace Everest;

class TowerDefense : public Layer {
    public:
        TowerDefense(const char* name);
        void onUpdate() override;
        void onGUIrender() override;
        void onEvent(Event& event) override;

    private:
        void handleEvents();

    private:
        OrthographicCameraController _camController;
        ref<VAO> _quad;
        ref<Shader> _shader;
        ref<Texture> _logo, _tiles;

        char _evLog[100] = "";
};
