#pragma once

#include <everest.h>

using namespace Everest;

class SandBox2D : public Layer {
    public:
        SandBox2D(const char* name);
        void onAttach() override;
        void onUpdate() override;
        void onGUIrender() override;
        void onEvent(Event& event) override;
        void onDetach() override;

    private:
        ref<Texture> _logo, _tiles;
        ivec2 _quads;
    private:
        OrthographicCameraController _camController;
};

