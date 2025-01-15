#pragma once

#include "math/types.h"
#include "yaml-cpp/emitter.h"

namespace Everest {

    enum class ForceMode {
        Force, Acceleration, Impulse, VelocityChange
    };

    struct rigidbody_c {
        vec3 velocity = vec3(0.f);
        f32 drag = 1.f;
        bool useGravity = true;

        void addForce(const vec3 force, const ForceMode mode = ForceMode::Force);
        void setMass(f32 value);
        inline void setInverseMass(f32 invm){_inverseMass = invm;}
        inline f32 getMass(){ return 1.f / _inverseMass; }

        private:
        f32 _inverseMass = 1.f;
        vec3 _forceAccumulator = vec3(0.f);
        vec3 _impulse = vec3(0.f);

        // performs integration and returns the change in distance
        vec3 integrate(const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody_c&);
    };

}
