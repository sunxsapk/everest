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
        char _evLog[100] = "";
};
