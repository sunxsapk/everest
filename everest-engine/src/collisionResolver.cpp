#include "physics/collisionResolver.h"
#include "math/utils.h"

namespace Everest {

    void body_contact_t::resolve(f32 duration){
    }

    void body_contact_t::resolveVelocity(f32 duration){
    }

    void body_contact_t::resolvePenetration(f32 duration){
    }

    f32 body_contact_t::getSeparationVelocity() const {
        return 0.f;
    }

    void body_contact2d_t::prepareContacts(){
        if(!rigidbody2dA){
            contactNormal = -contactNormal;
            std::swap(relativeContactPointA, relativeContactPointB);
            std::swap(rigidbody2dA, rigidbody2dB);
            std::swap(transformA, transformB);
        }
        ASSERT(rigidbody2dA != nullptr, "No rigidbodies for contact resolution");

        f32 raxn = Math::cross(relativeContactPointA, contactNormal);
        angularInertiaA = rigidbody2dA->inverseInertia * raxn * raxn;

        if(rigidbody2dB){
            f32 rbxn = Math::cross(relativeContactPointB, contactNormal);
            angularInertiaB = rigidbody2dB->inverseInertia * rbxn * rbxn;
        }
    }

    void body_contact2d_t::resolveVelocity(){
        vec2 relv = getRelativeVelocity();
        f32 svel = glm::dot(relv, contactNormal);
        if(svel >= 0.f) return;

        f32 totalInertia = rigidbody2dA->inverseMass + angularInertiaA;
        if(rigidbody2dB) totalInertia += rigidbody2dB->inverseMass + angularInertiaB;
        if(totalInertia <= 0.f) return;

        f32 jlen = -(1+restitution) * svel / totalInertia;
        vec2 j = jlen * contactNormal;

        rigidbody2dA->velocity += j * rigidbody2dA->inverseMass;
        rigidbody2dA->angularVelocity += Math::cross(relativeContactPointA, j) * rigidbody2dA->inverseInertia;

        if(rigidbody2dB){
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

        rigidbody2dA->velocity += jt * rigidbody2dA->inverseMass;
        rigidbody2dA->angularVelocity += Math::cross(relativeContactPointA, jt) * rigidbody2dA->inverseInertia;

        if(rigidbody2dB){
            rigidbody2dB->velocity -= jt * rigidbody2dB->inverseMass;
            rigidbody2dB->angularVelocity -= Math::cross(relativeContactPointB, jt) * rigidbody2dB->inverseInertia;
        }

    }

    void body_contact2d_t::resolvePenetration(){
        if(penetration <= 0) return;

        f32 totalInertia = rigidbody2dA->inverseMass + angularInertiaA;
        if(rigidbody2dB) totalInertia += rigidbody2dB->inverseMass + angularInertiaB;
        if(totalInertia <= 0.f) return;

        f32 dp = penetration / totalInertia;
        vec2 mvPerMass = contactNormal * dp;

        transformA->position += vec3(mvPerMass * rigidbody2dA->inverseMass, 0);
        transformA->rotation.z += dp * angularInertiaA;
        if(rigidbody2dB){
            transformB->position -= vec3(mvPerMass * rigidbody2dB->inverseMass, 0);
            transformB->rotation.z -= dp * angularInertiaB;
        }
    }

    f32 body_contact2d_t::getSeparationVelocity() const {
        return glm::dot(getRelativeVelocity(), contactNormal);
    }

    vec2 body_contact2d_t::getRelativeVelocity() const {
        vec2 relv(0.f);

        relv.x += rigidbody2dA->velocity.x - rigidbody2dA->angularVelocity * relativeContactPointA.y;
        relv.y += rigidbody2dA->velocity.y + rigidbody2dA->angularVelocity * relativeContactPointA.x;

        if(rigidbody2dB){
            relv.x -= rigidbody2dB->velocity.x - rigidbody2dB->angularVelocity * relativeContactPointB.y;
            relv.y -= rigidbody2dB->velocity.y + rigidbody2dB->angularVelocity * relativeContactPointB.x;
        }
        return relv;
    }

    contact_resolver_t::contact_resolver_t(u32 iterations)
        : _iterations(iterations){ }

    void contact_resolver_t::resolveContacts(std::vector<body_contact_t>& contactRegistry, f32 duration){
        if(!_iterations) _iterations = contactRegistry.size();
        _iterationsUsed = 0;
        while(_iterationsUsed++ < _iterations){
            f32 mx = 0.f;
            u32 mIndex = -1;

            for(int i=0; i<contactRegistry.size(); i++){
                f32 svel = contactRegistry[i].penetration;
                if(svel < mx){
                    mx = svel;
                    mIndex = i;
                }
            }

            if(mIndex == -1) return;
            contactRegistry[mIndex].resolve(duration);
        }
    }

    contact2d_resolver_t::contact2d_resolver_t(u32 iterations)
        : _iterations(iterations){ }

    void contact2d_resolver_t::resolvePenetration(std::vector<body_contact2d_t>& contactRegistry){
        // TODO: explore another method
        if(!_iterations) _iterations = contactRegistry.size();
        _iterationsUsed = 0;
        while(_iterationsUsed++ < _iterations){
            f32 mx = 0;
            u32 mIndex = -1;

            for(int i=0; i<contactRegistry.size(); i++){
                f32 svel = contactRegistry[i].getSeparationVelocity();
                if(svel < mx){
                    mx = svel;
                    mIndex = i;
                }
            }

            if(mIndex == -1) return;
            contactRegistry[mIndex].resolvePenetration();
        }
    }

    void contact2d_resolver_t::prepareContacts(std::vector<body_contact2d_t>& contactRegistry){
        for(auto& contact : contactRegistry){
            contact.prepareContacts();
        }
    }

    void contact2d_resolver_t::resolveVelocities(std::vector<body_contact2d_t>& contactRegistry){
        for(auto& contact : contactRegistry){
            contact.resolveVelocity();
        }
    }

    void contact2d_resolver_t::resolveContacts(std::vector<body_contact2d_t>& contactRegistry){
        prepareContacts(contactRegistry);
        resolvePenetration(contactRegistry);
        resolveVelocities(contactRegistry);
    }

}
