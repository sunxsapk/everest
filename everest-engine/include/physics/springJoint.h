#pragma once


#include "math/types.h"
#include "physics/rigidbody.h"

namespace Everest {

    struct springJoint_c {
        // TODO: store entity in runtime, which will be populated during serialization time
        // store reference to anchor *Entity*
        vec3 anchor = vec3(0.f);
        f32 springConstant = 1.f;
        f32 restLength = 1.f;

        void updateForce(vec3 otherPosition, rigidbody_c& otherBody);
    };

    struct springJoint2d_c {
        vec2 anchor = vec2(0.f); // store reference to anchor *Entity*
        f32 springConstant = 1.f;
        f32 restLength = 1.f;

        void updateForce(vec2 otherPosition, rigidbody2d_c& otherBody);
    };
}
