#include "physics/rigidbody.h"
#include "scene/components.h"

namespace Everest {

    void rigidbody_c::addForce(const vec3 value, const ForceMode mode){
        switch(mode){
            case ForceMode::Force:
                _forceAccumulator += value;
                break;
            case ForceMode::Acceleration:
                _forceAccumulator += value / inverseMass;
                break;
            case ForceMode::Impulse:
                _impulse += value;
                break;
            case ForceMode::VelocityChange:
                _impulse += value / inverseMass;
        }
    }

    void rigidbody_c::addForceAtOffset(const vec3 force, const vec3 offset){
        _torqueAccumulator += glm::cross(offset, force);
        _forceAccumulator += force;
    }

    void rigidbody_c::setMass(f32 value){
        ASSERT(value > 0.f, "Mass cannot be zero or negative");
        inverseMass = 1.f / value;
    }

    void rigidbody_c::integrate(transform_c& transform, const f32 timeStep){
        ASSERT(timeStep > 0.f, "Timestep for simulation cannot be negative or zero");

        transform.position += velocity * timeStep;

        _acceleration = (_forceAccumulator - drag * velocity) * inverseMass;
        vec3 iidv = _impulse * inverseMass;
        velocity += _acceleration * timeStep + iidv;
        _acceleration += iidv / timeStep;

        // inertia is assumed to be equal to mass... i.e. ms-radius = 1
        // Maybe, calculate it using what collider is used
        angularVelocity += (_torqueAccumulator - drag * angularVelocity) * inverseMass * timeStep;

        _forceAccumulator = vec3(0.f);
        _torqueAccumulator = vec3(0.f);
        _impulse = vec3(0.f);
    }


    void rigidbody2d_c::addForce(const vec2 value, const ForceMode mode){
        switch(mode){
            case ForceMode::Force:
                _forceAccumulator += value;
                break;
            case ForceMode::Acceleration:
                _forceAccumulator += value / inverseMass;
                break;
            case ForceMode::Impulse:
                _impulse += value;
                break;
            case ForceMode::VelocityChange:
                _impulse += value / inverseMass;
        }
    }

    void rigidbody2d_c::addForceAtOffset(const vec2 force, const vec2 offset){
        // we only need z component of torque for 2d world
        // TODO: optimizable
        //_torqueAccumulator += glm::cross(vec3(offset, 0), vec3(force, 0)).z;
        _torqueAccumulator += (offset.x * force.y - offset.y * force.x);
        _forceAccumulator += force;
    }

    void rigidbody2d_c::setMass(f32 value){
        ASSERT(value > 0.f, "Mass cannot be zero or negative");
        inverseMass = 1.f / value;
    }

    void rigidbody2d_c::integrate(transform_c& transform, const f32 timeStep){
        ASSERT(timeStep > 0.f, "Timestep for simulation cannot be negative or zero");

        transform.position += vec3(velocity * timeStep, 0.f);
        transform.rotation.z += glm::degrees(angularVelocity * timeStep);

        _acceleration = (_forceAccumulator - drag * velocity) * inverseMass;
        vec2 iidv = _impulse * inverseMass;
        velocity += _acceleration * timeStep + iidv;
        _acceleration += iidv / timeStep;

        // TODO: maybe calculate inertia using collider too
        angularVelocity += (_torqueAccumulator - drag * angularVelocity) * inverseMass * timeStep; // inertia is assumed to be equal to mass... i.e. ms-radius = 1

        _forceAccumulator = vec2(0.f);
        _torqueAccumulator = 0.f;
        _impulse = vec2(0.f);

    }
}
