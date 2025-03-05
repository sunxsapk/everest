#pragma once

#include "math/types.h"
#include "scene/component_type.h"

namespace Everest {

    struct circleRenderer_c : public component_t {
        vec4 color{1.f};
        f32 thickness = 1.f;
        f32 fade = 0.f;

        circleRenderer_c() = default;
        circleRenderer_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const circleRenderer_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }
    };

}
