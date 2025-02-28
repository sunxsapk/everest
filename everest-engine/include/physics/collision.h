#pragma once



#include "scene/entity.h"

namespace Everest {

    struct collision2d_t {
        Entity self, other;
        vec2 contact, normal;
        f32 penetration;
    };

}
