#pragma once

#include <everest.h>

using namespace Everest;

class TowerDefense : public Layer {
    public:
        TowerDefense(const char* name);
        void onUpdate() override;
        void onDebugRender() override;
        void onEvent(Event& event) override;

    private:
        void handleEvents();

    private:
        p_shared(VAO) _quad;
        p_shared(Shader) _shader;
        PerspectiveCamera _cam;

        char _evLog[100] = "";
};
