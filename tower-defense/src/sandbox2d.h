#pragma once

#include <everest.h>

using namespace Everest;

class SandBox2D : public Layer {
    public:
        SandBox2D(const char* name);
        void onAttach() override;
        void onUpdate() override;
        void onDebugRender() override;
        void onEvent(Event& event) override;
        void onDetach() override;

    private:
        ref<Texture> _logo, _tiles;
        vec4 color;
    private:
        OrthographicCameraController _camController;
};

