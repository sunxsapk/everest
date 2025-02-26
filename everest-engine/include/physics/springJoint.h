#pragma once


#include "math/types.h"
#include "physics/rigidbody.h"

namespace Everest {

    struct springJoint_c {
        // TODO: store entity in runtime, which will be populated during serialization time
        // store reference to anchor *Entity*
        vec3 anchor = vec3(0.f);
        vec3 offset = vec3(0.f);
        f32 springConstant = 20.f;
        f32 damping = 20.f;
        f32 restLength = 5.f;

        void updateForce(const transform_c& otherTransform, rigidbody_c& otherBody);
    };

    struct springJoint2d_c {
        vec2 anchor = vec2(0.f); // TODO: store reference to anchor *Entity*. DON'T forget to change lua_registry accordingly
        vec2 offset = vec2(0.f);
        f32 springConstant = 20.f;
        f32 damping = 20.f;
        f32 restLength = 5.f;

        void updateForce(const transform_c& otherTransform, rigidbody2d_c& otherBody);
    };
}
