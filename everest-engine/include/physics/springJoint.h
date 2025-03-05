#pragma once


#include "math/types.h"
#include "physics/rigidbody.h"

namespace Everest {

#ifndef __NO_3D__
    struct springJoint_c : public component_t {
        // TODO: store entity in runtime, which will be populated during serialization time
        // store reference to anchor *Entity*
        vec3 anchor = vec3(0.f);
        vec3 offset = vec3(0.f);
        f32 springConstant = 20.f;
        f32 damping = 20.f;
        f32 restLength = 5.f;

        springJoint_c(Entity ent) : component_t(ent) {}

        void updateForce(const transform_c& otherTransform, rigidbody_c& otherBody);
    };
#endif

    struct springJoint2d_c : public component_t {
        Entity anchor;
        vec2 offset = vec2(0.f);
        f32 springConstant = 20.f;
        f32 damping = 20.f;
        f32 restLength = 5.f;

        springJoint2d_c() = default;
        springJoint2d_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const springJoint2d_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }

        void updateForce(const transform_c& otherTransform, rigidbody2d_c& otherBody);
    };
}
