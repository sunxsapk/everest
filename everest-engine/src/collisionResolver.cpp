#include "physics/collisionResolver.h"
#include "math/utils.h"

namespace Everest {


#ifndef __NO_3D__
    void body_contact_t::prepareContacts(){
    }

    void body_contact_t::resolvePenetration(){
    }

    void body_contact_t::resolveVelocity(){
    }

    f32 body_contact_t::getSeparationVelocity() const{
        return glm::dot(getRelativeVelocity(), contactNormal);
    }

    vec3 body_contact_t::getRelativeVelocity() const{
        vec3 relv = rigidbodyA->velocity + glm::cross(rigidbodyA->angularVelocity, relativeContactPointA);
        if(rigidbodyB){
            relv += rigidbodyB->velocity + glm::cross(rigidbodyB->angularVelocity, relativeContactPointB);
        }
        return relv;
    }
#endif


    void body_contact2d_t::prepareContacts(){
        if(!rigidbody2dA){
            contactNormal = -contactNormal;
            std::swap(relativeContactPointA, relativeContactPointB);
            std::swap(rigidbody2dA, rigidbody2dB);
            std::swap(transformA, transformB);
        }
        ASSERT(rigidbody2dA != nullptr, "No rigidbodies for contact resolution");

        f32 raxn = Math::cross(relativeContactPointA, contactNormal);
        angularInertiaA = !(rigidbody2dA->definition&Static) ? rigidbody2dA->inverseInertia * raxn * raxn : 0;

        if(rigidbody2dB && !(rigidbody2dB->definition&Static)){
            f32 rbxn = Math::cross(relativeContactPointB, contactNormal);
            angularInertiaB = rigidbody2dB->inverseInertia * rbxn * rbxn;
        }
    }

    void body_contact2d_t::resolveVelocity(){
        vec2 relv = getRelativeVelocity();
        f32 svel = glm::dot(relv, contactNormal);
        if(svel >= 0.1f) return;

        bool adynamic = !(rigidbody2dA->definition&Static);
        bool bdynamic = rigidbody2dB && !(rigidbody2dB->definition&Static);

        f32 totalInertia = adynamic?rigidbody2dA->inverseMass + angularInertiaA:0;
        if(bdynamic) totalInertia += rigidbody2dB->inverseMass + angularInertiaB;
        if(totalInertia <= 0.f) return;

        f32 jlen = -(1+restitution) * svel / totalInertia;
        vec2 j = jlen * contactNormal;

        if(adynamic){
            rigidbody2dA->velocity += j * rigidbody2dA->inverseMass;
            rigidbody2dA->angularVelocity += Math::cross(relativeContactPointA, j) * rigidbody2dA->inverseInertia;
        }

        if(bdynamic){
            rigidbody2dB->velocity -= j * rigidbody2dB->inverseMass;
            rigidbody2dB->angularVelocity -= Math::cross(relativeContactPointB, j) * rigidbody2dB->inverseInertia;
        }

        vec2 vt = relv - svel * contactNormal;
        if(glm::length(vt) < 0.001f) return; 

        vt = glm::normalize(vt);
        f32 jtlen = -glm::dot(vt, relv) / totalInertia;
        f32 maxFriction = friction * jlen;
        jtlen = glm::clamp(jtlen, -maxFriction, maxFriction);
        vec2 jt = jtlen * vt;

        if(adynamic){
            rigidbody2dA->velocity += jt * rigidbody2dA->inverseMass;
            rigidbody2dA->angularVelocity += Math::cross(relativeContactPointA, jt) * rigidbody2dA->inverseInertia;
        }

        if(bdynamic){
            rigidbody2dB->velocity -= jt * rigidbody2dB->inverseMass;
            rigidbody2dB->angularVelocity -= Math::cross(relativeContactPointB, jt) * rigidbody2dB->inverseInertia;
        }

    }

    void body_contact2d_t::resolvePenetration(){
        if(penetration <= 0) return;

        bool adynamic = !(rigidbody2dA->definition&Static);
        bool bdynamic = rigidbody2dB && !(rigidbody2dB->definition&Static);

        f32 totalInertia = adynamic?rigidbody2dA->inverseMass:0 + angularInertiaA;
        if(bdynamic) totalInertia += rigidbody2dB->inverseMass + angularInertiaB;
        if(totalInertia <= 0.f) return;

        const f32 PEN_BIAS = 0.1f;
        f32 dp = penetration / totalInertia * PEN_BIAS;
        vec2 mvPerMass = contactNormal * dp;

        if(adynamic) {
            transformA->position += vec3(mvPerMass * rigidbody2dA->inverseMass, 0);
            transformA->rotation.z += dp * angularInertiaA;
        }
        if(bdynamic){
            transformB->position -= vec3(mvPerMass * rigidbody2dB->inverseMass, 0);
            transformB->rotation.z -= dp * angularInertiaB;
        }
    }

    f32 body_contact2d_t::getSeparationVelocity() const {
        return glm::dot(getRelativeVelocity(), contactNormal);
    }

    vec2 body_contact2d_t::getRelativeVelocity() const {
        vec2 relv(0.f);

        if(!(rigidbody2dA->definition&Static)){
            relv.x += rigidbody2dA->velocity.x - rigidbody2dA->angularVelocity * relativeContactPointA.y;
            relv.y += rigidbody2dA->velocity.y + rigidbody2dA->angularVelocity * relativeContactPointA.x;
        }

        if(rigidbody2dB && !(rigidbody2dB->definition&Static)){
            relv.x -= rigidbody2dB->velocity.x - rigidbody2dB->angularVelocity * relativeContactPointB.y;
            relv.y -= rigidbody2dB->velocity.y + rigidbody2dB->angularVelocity * relativeContactPointB.x;
        }
        return relv;
    }

#ifndef __NO_3D__
    void contact_resolver_t::resolvePenetration(std::vector<body_contact_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.resolvePenetration();
        }
    }

    void contact_resolver_t::prepareContacts(std::vector<body_contact_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.prepareContacts();
        }
    }

    void contact_resolver_t::resolveVelocities(std::vector<body_contact_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.resolveVelocity();
        }
    }

    void contact_resolver_t::resolveContacts(std::vector<body_contact_t>& contactRegistry){
        EV_profile_function();
        if(contactRegistry.size() == 0) return;
        std::sort(contactRegistry.begin(), contactRegistry.end(),
                [](body_contact_t& a, body_contact_t& b){ return a.penetration > b.penetration; });
        prepareContacts(contactRegistry);
        resolvePenetration(contactRegistry);
        resolveVelocities(contactRegistry);
    }
#endif


    void contact2d_resolver_t::prepareContacts(std::vector<body_contact2d_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.prepareContacts();
        }
    }

    void contact2d_resolver_t::resolvePenetration(std::vector<body_contact2d_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.resolvePenetration();
        }
    }

    void contact2d_resolver_t::resolveVelocities(std::vector<body_contact2d_t>& contactRegistry){
        EV_profile_function();
        for(auto& contact : contactRegistry){
            contact.resolveVelocity();
        }
    }

    void contact2d_resolver_t::resolveContacts(std::vector<body_contact2d_t>& contactRegistry){
        EV_profile_function();
        if(contactRegistry.size() == 0) return;
        prepareContacts(contactRegistry);
        resolvePenetration(contactRegistry);
        resolveVelocities(contactRegistry);
    }

}
