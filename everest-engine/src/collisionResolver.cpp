#include "physics/collisionResolver.h"

namespace Everest {

    void BodyContact::resolve(f32 duration){
        resolveVelocity(duration);
        resolvePenetration(duration);
    }

    void BodyContact::resolveVelocity(f32 duration){
        f32 svel = calcSeparateVelocity();
        if(svel > 0.f) return;
        f32 tim = body1.inverseMass + body2.inverseMass;
        if(tim <= 0.f) return;

        f32 nsvel = - svel * restitution;
        vec3 accvel = body1.getAcceleration() - body2.getAcceleration();
        f32 acsv = glm::dot(accvel, contactNormal) * duration;
        if(acsv < 0.f) nsvel = glm::max(0.f, nsvel + restitution * acsv);

        f32 dvel = nsvel - svel;

        f32 impulse = dvel / tim;
        vec3 impulsePerInvMass = contactNormal * impulse;
        body1.velocity = body1.velocity + impulsePerInvMass * body1.inverseMass;
        body2.velocity = body2.velocity - impulsePerInvMass * body2.inverseMass;
    }

    void BodyContact::resolvePenetration(f32 duration){
        if(penetration <= 0) return;

        f32 tim = body1.inverseMass + body2.inverseMass;
        if(tim <= 0.f) return;

        vec3 mvPerMass = contactNormal * (-penetration / tim);
        transform1.position += mvPerMass * body1.inverseMass;
        transform2.position += mvPerMass * body2.inverseMass;
    }

    f32 BodyContact::calcSeparateVelocity() const {
        vec3 relv = body1.velocity - body2.velocity;
        return glm::dot(relv, contactNormal);
    }

    void BodyContact2D::resolve(f32 duration){
        resolveVelocity(duration);
        resolvePenetration(duration);
    }

    void BodyContact2D::resolveVelocity(f32 duration){
        f32 svel = calcSeparateVelocity();
        if(svel > 0.f) return;
        f32 tim = body1.inverseMass + body2.inverseMass;
        if(tim <= 0.f) return;

        f32 nsvel = - svel * restitution;
        f32 dvel = nsvel - svel;

        f32 impulse = dvel / tim;
        vec2 impulsePerInvMass = contactNormal * impulse;
        body1.velocity = body1.velocity + impulsePerInvMass * body1.inverseMass;
        body2.velocity = body2.velocity - impulsePerInvMass * body2.inverseMass;
    }

    void BodyContact2D::resolvePenetration(f32 duration){
        if(penetration <= 0) return;

        f32 tim = body1.inverseMass + body2.inverseMass;
        if(tim <= 0.f) return;

        vec2 mvPerMass = contactNormal * (-penetration / tim);
        transform1.position += vec3(mvPerMass * body1.inverseMass, 0);
        transform2.position += vec3(mvPerMass * body2.inverseMass, 0);
    }

    f32 BodyContact2D::calcSeparateVelocity() const {
        vec2 relv = body1.velocity - body2.velocity;
        return glm::dot(relv, contactNormal);
    }


    ContactResolver::ContactResolver(u32 iterations)
        : _iterations(iterations){ }

    void ContactResolver::resolveContacts(std::vector<BodyContact>& contactRegistry, f32 duration){
        if(!_iterations) _iterations = contactRegistry.size();
        _iterationsUsed = 0;
        while(_iterationsUsed++ < _iterations){
            f32 mx = 0;
            u32 mIndex = -1;

            for(int i=0; i<contactRegistry.size(); i++){
                f32 svel = contactRegistry[i].calcSeparateVelocity();
                if(svel < mx){
                    mx = svel;
                    mIndex = i;
                }
            }

            if(mIndex == -1) return;
            contactRegistry[mIndex].resolve(duration);
        }
    }

    ContactResolver2D::ContactResolver2D(u32 iterations)
        : _iterations(iterations){ }

    void ContactResolver2D::resolveContacts(std::vector<BodyContact2D>& contactRegistry, f32 duration){
        if(!_iterations) _iterations = contactRegistry.size();
        _iterationsUsed = 0;
        while(_iterationsUsed++ < _iterations){
            f32 mx = 0;
            u32 mIndex = -1;

            for(int i=0; i<contactRegistry.size(); i++){
                f32 svel = contactRegistry[i].calcSeparateVelocity();
                if(svel < mx){
                    mx = svel;
                    mIndex = i;
                }
            }

            if(mIndex == -1) return;
            contactRegistry[mIndex].resolve(duration);
        }
    }

}
