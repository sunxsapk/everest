#pragma once

#include "math/types.h"
#include "yaml-cpp/emitter.h"

namespace Everest {
    struct transform_c;

    enum class ForceMode {
        Force, Acceleration, Impulse, VelocityChange
    };

    struct rigidbody_c {
        vec3 velocity = vec3(0.f);
        vec3 angularVelocity = vec3(0.f);
        f32 drag = 1.f;
        f32 inverseMass = 1.f;
        f32 inverseInertia = 1.f;
        bool useGravity = true;

        void addForce(const vec3 force, const ForceMode mode = ForceMode::Force);
        void addForceAtOffset(const vec3 force, const vec3 offset);
        void setMass(f32 value);
        inline f32 getMass(){ return 1.f / inverseMass; }
        inline vec3 getAcceleration() const {return _acceleration;}

        private:
        vec3 _forceAccumulator = vec3(0.f);
        vec3 _torqueAccumulator = vec3(0.f);
        vec3 _impulse = vec3(0.f);
        vec3 _acceleration = vec3(0.f);

        void integrate(transform_c& transform, const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody_c&);
    };


    struct rigidbody2d_c {
        vec2 velocity = vec2(0.f);
        f32 angularVelocity = 0.f;
        f32 drag = 1.f;
        f32 inverseMass = 1.f;
        f32 inverseInertia = 1.f;
        bool useGravity = true;

        void addForce(const vec2 force, const ForceMode mode = ForceMode::Force);
        void addForceAtOffset(const vec2 force, const vec2 offset);
        void setMass(f32 value);
        inline f32 getMass(){ return 1.f / inverseMass; }
        inline vec2 getAcceleration() const {return _acceleration;}

        private:
        vec2 _forceAccumulator = vec2(0.f);
        f32 _torqueAccumulator = 0.f;
        vec2 _impulse = vec2(0.f);
        vec2 _acceleration = vec2(0.f);

        void integrate(transform_c& transform, const f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody2d_c&);
    };

}
