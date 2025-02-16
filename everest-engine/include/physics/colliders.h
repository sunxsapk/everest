/*
 * ====== Colliders ===========
 * Author: Sunil Sapkota
 * Description: Colliders are some of the basic shapes which are used for collision detection and
 * resolution in physics engine
 */

#pragma once

#include "scene/entity.h"

namespace Everest {

    enum class ColliderType {
        Box,
        Circle
    };

    //TODO: test 2d colliders first and then work on 3d colliders 
    struct collider_c {
        Entity entity;
        ColliderType type;
        f32 restitution = 1.f;
    };
}
