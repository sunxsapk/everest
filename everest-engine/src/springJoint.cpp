#include "physics/springJoint.h"



namespace Everest {

    void springJoint_c::updateForce(vec3 otherPosition, rigidbody_c& otherBody){
        vec3 force = otherPosition - anchor;
        f32 mag = glm::length(force);
        mag = abs(mag - restLength) * springConstant;
        force = glm::normalize(force);
        force *= -mag;
        otherBody.addForce(force);
    }

    void springJoint2d_c::updateForce(vec2 otherPosition, rigidbody2d_c& otherBody){
        vec2 force = otherPosition - anchor;
        f32 mag = glm::length(force);
        mag = abs(mag - restLength) * springConstant;
        force = glm::normalize(force);
        force *= -mag;
        otherBody.addForce(force);
    }
}
