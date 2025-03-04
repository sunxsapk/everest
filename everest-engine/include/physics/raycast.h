/*
 * ======= Raycast =========
 * Author: Sunil Sapkota
 * Description: Raycasting is a very powerful mathematical tool provided by everest engine, which
 * casts a ray from a certain position towards certain direction and checks which colliders did the
 * ray collide with.
 */


#pragma once

#include "math/types.h"
#include "scene/entity.h"

namespace Everest {

    struct Ray2D {
        vec2 origin, direction;
        f32 maxLength = -1.f; // negative implies no maximum length
    };

    struct RaycastResult2D {
        Entity hitEntity;
        vec2 hitpoint;
        bool hit = false;
    };

}
