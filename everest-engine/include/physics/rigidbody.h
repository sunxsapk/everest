#pragma once

#include "math/types.h"
#include "yaml-cpp/emitter.h"
#include "scene/component_type.h"


namespace Everest {
    struct transform_c;

    enum BodyDefBits {
        UseGravity = 1 << 0,
        Static = 1 << 1,
    };

    enum class ForceMode {
        Force = 0, Acceleration, Impulse, VelocityChange
    };

#ifndef __NO_3D__
    struct rigidbody_c : public component_t {
        vec3 velocity = vec3(0.f);
        vec3 angularVelocity = vec3(0.f);
        f32 drag = 1.f;
        f32 inverseMass = 1.f;
        f32 inverseInertia = 1.f;
        bool useGravity = true;

        rigidbody_c(Entity ent) : component_t(ent) {}

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
#endif

    struct rigidbody2d_c : public component_t {
        vec2 velocity = vec2(0.f);
        f32 angularVelocity = 0.f;
        f32 drag = 1.f;
        f32 inverseMass = 1.f;
        f32 inverseInertia = 1.f;
        int definition = BodyDefBits::UseGravity;

        rigidbody2d_c() = default;
        rigidbody2d_c(Entity ent) : component_t(ent) {}
        void makeCopyUsing(const rigidbody2d_c& other, Entity ent){
            *this = other;
            entity = ent;
            active = other.active;
        }

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

        void integrate(transform_c& transform, f32 timeStep);
        friend class PhysicsHandler;
        friend YAML::Emitter& operator<<(YAML::Emitter&, const rigidbody2d_c&);
    };

}
