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

    enum class collider2d_type_t {
        Box,
        Circle
    };


    struct box2dprops_t {
        vec2 offset = vec2(0.f);
        vec2 halfExtents = vec2(0.5f);

        bool contains(vec2 point);
    };

    struct circleprops_t {
        vec2 offset = vec2(0.0f);
        f32 radius = 0.5f;
    };

    struct boxCollider2d_c {
        box2dprops_t box;

        f32 restitution = 0.5f;
    };

    struct circleCollider2d_c {
        circleprops_t circle;

        f32 restitution = 0.5f;
    };

    struct collider2d_c {
        Entity entity;
        collider2d_type_t type;
        f32 restitution = 1.f;

        union Props {
            box2dprops_t box;
            circleprops_t circle;

            constexpr Props() : box{}{}
        } props;

        collider2d_c(collider2d_type_t type);
        collider2d_c(const boxCollider2d_c& boxCollider, Entity entity);
        collider2d_c(const circleCollider2d_c& circleCollider, Entity entity);
        aabb2d_t getBounds();
    };

}

