#pragma once



#include "scene/entity.h"

namespace Everest {

    struct Collision2D {
        Entity other;
        vec2 contact, normal;
        f32 penetration;
    };

}
