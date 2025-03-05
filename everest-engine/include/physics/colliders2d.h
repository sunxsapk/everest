/*
 * ====== Colliders2D ===========
 * Author: Sunil Sapkota
 * Description: Colliders2d are some of the basic 2d shapes which are used for collision detection and
 * resolution in physics engine
 */

#pragma once


#include "math/types.h"
#include "aabbtree2d.h"
#include "scene/component_type.h"
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

        bool contains(vec2 point);
    };

    struct boxCollider2d_c : public component_t {
        box2dprops_t box;

        boxCollider2d_c() = default;
        boxCollider2d_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const boxCollider2d_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }

        f32 restitution = 0.5f;
    };

    struct circleCollider2d_c : public component_t {
        circleprops_t circle;

        circleCollider2d_c() = default;
        circleCollider2d_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const circleCollider2d_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }

        f32 restitution = 0.5f;
    };

    struct collider2d_t {
        Entity entity;
        collider2d_type_t type;
        f32 restitution = 1.f;

        union Props {
            box2dprops_t box;
            circleprops_t circle;

            constexpr Props() : box{}{}
        } props;

        collider2d_t(collider2d_type_t type);
        collider2d_t(const boxCollider2d_c& boxCollider, Entity entity);
        collider2d_t(const circleCollider2d_c& circleCollider, Entity entity);
        aabb2d_t getBounds();
    };

}

