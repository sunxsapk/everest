#pragma once

#include <everest.h>

class TowerDefense : public Everest::Layer {
    public:
        TowerDefense(const char* name);
        void onUpdate() override;
        void onDebugRender() override;
};
