#pragma once

#include "rigidbody.h"
#include "math/types.h"

namespace Everest {

    struct rigidbody2d_c {
        vec2 velocity = vec2(0.f);
        f32 drag = 1.f;
        bool useGravity = true;

        void addForce(const vec2 force, const ForceMode mode = ForceMode::Force);
        void setMass(f32 value);
        inline void setInverseMass(f32 invm){_inverseMass = invm;}
        inline f32 getMass(){ return 1.f / _inverseMass; }

        private:
        f32 _inverseMass = 1.f;
        vec2 _forceAccumulator = vec2(0.f);
        vec2 _impulse = vec2(0.f);

        // performs integration and returns the change in distance
        vec2 integrate(const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody2d_c&);
    };

}
