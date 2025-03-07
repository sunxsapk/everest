#pragma once

#include "math/types.h"
#include "renderer/texture.h"
#include "scene/component_type.h"

namespace Everest {

    struct circleRenderer_c : public component_t {
        ref<Texture> texture = nullptr;
        vec4 color{1.f};
        vec2 startUV = vec2(0.f);
        vec2 sizeUV  = vec2(1.f);
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
