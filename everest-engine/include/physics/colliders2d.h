/*
 * ====== Colliders2D ===========
 * Author: Sunil Sapkota
 * Description: Colliders2d are some of the basic 2d shapes which are used for collision detection and
 * resolution in physics engine
 */

#pragma once


#include "math/types.h"
#include "aabbtree2d.h"
#include "scene/entity.h"

namespace Everest {

    enum class Collider2DType {
        Box,
        Circle
    };


    struct Box2DProps {
        vec2 offset = vec2(0.f);
        vec2 halfExtents = vec2(0.5f);
    };

    struct CircleProps {
        vec2 offset = vec2(0.0f);
        f32 radius = 0.5f;
    };

    struct boxCollider2d_c {
        Box2DProps box;
        f32 restitution = 1.f;
    };

    struct circleCollider2d_c {
        CircleProps circle;
        f32 restitution = 1.f;
    };

    struct collider2d_c {
        Entity entity;
        Collider2DType type;
        f32 restitution = 1.f;

        union Props {
            Box2DProps box;
            CircleProps circle;

            constexpr Props() : box{}{}
        } props;

        collider2d_c(Collider2DType type);
        collider2d_c(const boxCollider2d_c& boxCollider, Entity entity);
        collider2d_c(const circleCollider2d_c& circleCollider, Entity entity);
        AABB2D getBounds();
    };

}

