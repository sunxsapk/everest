#include "physics/rigidbody2d.h"

namespace Everest {

    void rigidbody2d_c::addForce(const vec2 value, const ForceMode mode){
        switch(mode){
            case ForceMode::Force:
                _forceAccumulator += value;
                break;
            case ForceMode::Acceleration:
                _forceAccumulator += value / _inverseMass;
                break;
            case ForceMode::Impulse:
                _impulse += value;
                break;
            case ForceMode::VelocityChange:
                _impulse += value / _inverseMass;
        }
    }

    void rigidbody2d_c::setMass(f32 value){
        ASSERT(value > 0.f, "Mass cannot be zero or negative");
        _inverseMass = 1.f / value;
    }

    vec2 rigidbody2d_c::integrate(const f32 timeStep){
        ASSERT(timeStep > 0.f, "Timestep for simulation cannot be negative or zero");

        vec2 dp = velocity * timeStep;
        vec2 acc = (_forceAccumulator - drag * velocity) * _inverseMass;
        velocity += acc * timeStep + _impulse * _inverseMass;

        _forceAccumulator = vec2(0.f);
        _impulse = vec2(0.f);

        return dp;
    }
}
