#include "physics/rigidbody.h"
#include "scene/components.h"

#undef min
#undef max

namespace Everest {

/*#ifndef __NO_3D__
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
        ASSERT(timeStep >= 0.f, "Timestep for simulation cannot be negative");

        transform.position += velocity * timeStep;

        _acceleration = (_forceAccumulator - drag * velocity) * inverseMass;
        vec3 iidv = _impulse * inverseMass;
        velocity += _acceleration * timeStep + iidv;
        _acceleration += iidv / timeStep;

        angularVelocity += (_torqueAccumulator - drag * angularVelocity) * inverseInertia * timeStep;

        _forceAccumulator = vec3(0.f);
        _torqueAccumulator = vec3(0.f);
        _impulse = vec3(0.f);
    }
#endif*/


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
        _torqueAccumulator += (offset.x * force.y - offset.y * force.x);
        _forceAccumulator += force;
    }

    void rigidbody2d_c::setMass(f32 value){
        ASSERT(value > 0.f, "Mass cannot be zero or negative");
        inverseMass = 1.f / value;
    }

    void rigidbody2d_c::integrate(transform_c& transform, f32 timeStep){
        timeStep = std::max(0.00001f, timeStep);
        if((definition & BodyDefBits::Static) == 0){
            transform.position += vec3(velocity * timeStep, 0.f);
            transform.rotation.z += glm::degrees(angularVelocity * timeStep);

            _acceleration = (_forceAccumulator - drag * velocity) * inverseMass;
            vec2 iidv = _impulse * inverseMass;
            velocity += _acceleration * timeStep + iidv;
            _acceleration += iidv / timeStep;

            angularVelocity += (_torqueAccumulator - drag * angularVelocity) * inverseInertia * timeStep;
        }

        _forceAccumulator = vec2(0.f);
        _torqueAccumulator = 0.f;
        _impulse = vec2(0.f);

    }
}
