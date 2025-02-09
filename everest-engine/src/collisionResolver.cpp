#include "physics/collisionResolver.h"
#include "math/utils.h"

namespace Everest {

    void BodyContact::resolve(f32 duration){
        if(!body1 && !body2) return;
        resolvePenetration(duration);
        resolveVelocity(duration);
    }

    void BodyContact::resolveVelocity(f32 duration){
        f32 svel = calcSeparateVelocity();
        if(svel > 0.f) return;
        f32 t_im = body1?body1->inverseMass:0.f;
        if(body2) t_im += body2->inverseMass;
        if(t_im <= 0.f) return;

        f32 nsvel = -svel * restitution;
        vec3 accvel = body1?body1->getAcceleration():vec3(0);
        if(body2) accvel -= body2->getAcceleration();
        f32 acsv = glm::dot(accvel, contactNormal) * duration;
        if(acsv < 0.f) nsvel = glm::max(0.f, nsvel + restitution * acsv);

        f32 dvel = nsvel - svel;

        f32 impulse = dvel / t_im;
        vec3 impulsePerInvMass = contactNormal * impulse;
        if(body1) body1->velocity += impulsePerInvMass * body1->inverseMass;
        if(body2) body2->velocity -= impulsePerInvMass * body2->inverseMass;
    }

    void BodyContact::resolvePenetration(f32 duration){
        if(penetration <= 0) return;

        f32 t_im = (body1?body1->inverseMass:0);
        if(body2) t_im += body2->inverseMass;
        if(t_im <= 0.f) return;

        vec3 mvPerMass = contactNormal * (penetration / t_im);
        if(body1) transform1->position += mvPerMass * body1->inverseMass;
        if(body2) transform2->position -= mvPerMass * body2->inverseMass;
    }

    f32 BodyContact::calcSeparateVelocity() const {
        vec3 relv = body1?body1->velocity:vec3(0) - (body2?body2->velocity:vec3(0));
        return glm::dot(relv, contactNormal);
    }

    void BodyContact2D::resolve(f32 duration){
        if(!body1 && !body2) return;
        resolvePenetration(duration);
        resolveVelocity(duration);
    }

    /*void BodyContact2D::resolveVelocity(f32 duration){
        // TODO: this whole thing
        bool _b1 = body1 != nullptr;
        bool _b2 = body2 != nullptr;

        vec2 relv(0.f);
        if(_b1) relv += body1->velocity + body1->angularVelocity * ra;
        if(_b2) relv -= body2->velocity + body2->angularVelocity * rb;
        f32 svel = glm::dot(relv, contactNormal);
        if(svel > 0.f) return;

        f32 t_im = 0;
        if(_b1) t_im += body1->inverseMass;
        if(_b2) t_im += body2->inverseMass;
        if(t_im <= 0.f) return;

        f32 t_ii = 0;
        f32 ran = 0, rbn = 0;
        if(_b1) {
            ran = (ra.x * contactNormal.y - ra.y * contactNormal.x);
            t_ii += ran * ran * body1->inverseMass;
        }
        if(_b2) {
            rbn = (rb.x * contactNormal.y - rb.y * contactNormal.x);
            t_ii += rbn * rbn * body2->inverseMass;
        }

        f32 impulse = -(1+restitution) * svel / (t_im + t_ii);
        vec2 impulsePerInvMass = contactNormal * impulse;
        if(_b1){
            body1->velocity += impulsePerInvMass * body1->inverseMass;
            body1->angularVelocity += ran * impulse * body1->inverseMass;
        }
        if(_b2){
            body2->velocity -= impulsePerInvMass * body2->inverseMass;
            body2->angularVelocity -= rbn * impulse * body2->inverseMass;
        }

        vec2 tangent = glm::normalize(relv - glm::dot(relv, contactNormal) * contactNormal);
        f32 _fr_impulse = friction * impulse;

        t_ii = 0;
        if(_b1){
            ran = (ra.x * tangent.y - ra.y * tangent.x);
            t_ii += ran * ran * body1->inverseMass;
        }
        if(_b2){
            ran = (rb.x * tangent.y - rb.y * tangent.x);
            t_ii += rbn * rbn * body2->inverseMass;
        }

        _fr_impulse = glm::clamp(-glm::dot(relv, tangent) / (t_im + t_ii), -_fr_impulse, _fr_impulse);
        impulsePerInvMass = _fr_impulse * tangent;
        EVLog_Wrn("{%f, %f}", impulsePerInvMass.x, impulsePerInvMass.y);
        if(_b1){
            body1->velocity += impulsePerInvMass * body1->inverseMass;
            body1->angularVelocity += ran * _fr_impulse * body1->inverseMass;
        }
        if(_b2){
            body2->velocity -= impulsePerInvMass * body2->inverseMass;
            body2->angularVelocity -= rbn * _fr_impulse * body2->inverseMass;
        }
    }


        f32 nsvel = - svel * restitution;
        f32 dvel = nsvel - svel;

        f32 impulse = dvel / tim;
        vec2 impulsePerInvMass = contactNormal * impulse;
        if(body1) {
            body1->velocity += impulsePerInvMass * body1->inverseMass;
            body1->angularVelocity += Math::cross(ra, impulsePerInvMass) * body1->inverseInertia;
        }
        if(body2) {
            body2->velocity -= impulsePerInvMass * body2->inverseMass;
            body2->angularVelocity -= Math::cross(rb, impulsePerInvMass) * body2->inverseInertia;
        }

*/

    void BodyContact2D::resolveVelocity(f32 duration){
        vec2 relv = calcRelativeVelocity();
        f32 svel = glm::dot(relv, contactNormal);
        if(svel > 0.f) return;

        f32 raxn = Math::cross(ra, contactNormal);
        f32 rbxn = Math::cross(rb, contactNormal);

        f32 tim = 0;
        if(body1){
            tim += body1->inverseMass + raxn * raxn * body1->inverseInertia;
        }
        if(body2){
            tim += body2->inverseMass + rbxn * rbxn * body2->inverseInertia;
        }
        if(tim <= 0.f) return;

        vec2 j = -(1+restitution) * svel / tim * contactNormal;
        if(body1){
            body1->velocity += j * body1->inverseMass;
            body1->angularVelocity += Math::cross(ra, j) * body1->inverseInertia;
        }
        if(body2){
            body2->velocity -= j * body2->inverseMass;
            body2->angularVelocity -= Math::cross(rb, j) * body2->inverseInertia;
        }
    }

    void BodyContact2D::resolvePenetration(f32 duration){
        // TODO: penetration is not resolved properly when one box is at edge of other
        if(penetration <= 0) return;

        f32 t_im = body1?body1->inverseMass:0;
        if(body2) t_im += body2->inverseMass;
        if(t_im <= 0.f) return;

        vec2 mvPerMass = contactNormal * (penetration / t_im);
        if(body1) transform1->position += vec3(mvPerMass * body1->inverseMass, 0);
        if(body2) transform2->position -= vec3(mvPerMass * body2->inverseMass, 0);
    }

    f32 BodyContact2D::calcSeparateVelocity() const {
        return glm::dot(calcRelativeVelocity(), contactNormal);
    }

    vec2 BodyContact2D::calcRelativeVelocity() const {
        vec2 relv(0.f);
        if(body1){
            relv.x += body1->velocity.x - body1->angularVelocity * ra.y;
            relv.y += body1->velocity.y + body1->angularVelocity * ra.x;
        }

        if(body2){
            relv.x -= body2->velocity.x - body2->angularVelocity * rb.y;
            relv.y -= body2->velocity.y + body2->angularVelocity * rb.x;
        }
        return relv;
    }

    ContactResolver::ContactResolver(u32 iterations)
        : _iterations(iterations){ }

    void ContactResolver::resolveContacts(std::vector<BodyContact>& contactRegistry, f32 duration){
        // TODO!: search for optimal way. This is shit way
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
        // TODO!: search for optimal way. This is shit way
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
