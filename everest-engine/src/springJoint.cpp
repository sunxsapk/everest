#include "physics/springJoint.h"
#include "scene/components.h"



namespace Everest {

/*#ifndef __NO_3D__
    void springJoint_c::updateForce(const transform_c& otherTransform, rigidbody_c& otherBody){
        transform_c tmpt = otherTransform;
        tmpt.position = {};
        vec3 noff = (mat4)tmpt * vec4(offset, 0);
        vec3 otherPosition = otherTransform.position + noff - anchor;
        vec3 sf = -springConstant * glm::normalize(otherPosition) * 
            (glm::length(otherPosition) - restLength);
        vec3 df = -damping * otherBody.velocity;

        otherBody.addForceAtOffset(sf + df, noff);
    }
#endif*/

    void springJoint2d_c::updateForce(const transform_c& otherTransform, rigidbody2d_c& otherBody){
        if(!anchor.isValid()) return;

        vec2 anPos = anchor.get<transform_c>().position;

        transform_c tmpt = otherTransform;
        tmpt.position = {};
        vec2 noff = (mat4)tmpt * vec4(offset, 0, 0);
        vec2 otherPosition = otherTransform.position;
        otherPosition += noff - anPos;

        vec2 sf = -springConstant * glm::normalize(otherPosition) * 
            (glm::length(otherPosition) - restLength);
        vec2 df = -damping * otherBody.velocity;

        otherBody.addForceAtOffset(sf + df, noff);
    }
}
