#include "physics/physicsworld.h"
#include "physics/aabbtree2d.h"
#include "physics/collision.h"
#include "scene/components.h"
#include "physics/collisionDetector.h"

#ifndef __NO_3D__
#include "physics/aabbtree.h"
#include "physics/colliders.h"
#endif

namespace Everest {

    u32 PhysicsHandler::_simulationSteps = 4;
    vec3 PhysicsHandler::_gravity = vec3(0.f, -9.8f, 0.f);
    contact2d_resolver_t PhysicsHandler::_contactResolver2d;
    aabb2d_tree_t<collider2d_c> PhysicsHandler::tree2d;
    std::vector<body_contact2d_t> PhysicsHandler::contacts2d(16);
    std::vector<collision2d_t> PhysicsHandler::collision2dResults(16);

#ifndef __NO_3D__
    contact_resolver_t PhysicsHandler::_contactResolver3d;
    std::vector<body_contact_t> PhysicsHandler::contacts(16);
    aabb_tree_t<collider_c> PhysicsHandler::tree2d;
#endif

    void PhysicsHandler::init(u32 simulationSteps, vec3 gravity) {
        EV_profile_function();
        _simulationSteps = simulationSteps;
        _gravity = gravity;
    }

    void PhysicsHandler::quit(){
        EV_profile_function();
    }

    void PhysicsHandler::simulate(Scene& scene, f64 timeStep){
        EV_profile_function();
        simulate2d(scene, timeStep);
#ifndef __NO_3D__
        simulate3d(scene, timeStep);
#endif
    }

#ifndef __NO_3D__
    void PhysicsHandler::generateContacts(Scene& scene, f64 timeStep){
        EV_profile_function();
        static std::vector<aabb_t> colliderBounds(16);
        static aabb_tree_t<collider_c> tree;

        tree.clear();
        contacts.clear();
        colliderBounds.clear();

        // TODO: 3d collision detection and resolution
    }



    void PhysicsHandler::simulate3d(Scene& scene, f64 timeStep){
        EV_profile_function();
        registry_t& registry = scene._registry;
        auto spjgrp = registry.group<springJoint_c>(entt::get<transform_c, rigidbody_c>);
        auto phygrp = registry.group<rigidbody_c>(entt::get<transform_c>);

        for(auto ent:spjgrp){
            const auto& [spr, tfr, rb] = spjgrp.get(ent);
            spr.updateForce(tfr, rb);
        }

        for(auto ent:phygrp){
            const auto& [rb, tfr] = phygrp.get(ent);
            if(rb.useGravity) rb.addForce(_gravity, ForceMode::Acceleration);
            rb.integrate(tfr, timeStep);
        }

        generateContacts(scene, timeStep);
        _contactResolver3d.resolveContacts(contacts);
    }

#endif

    void PhysicsHandler::generateContacts2d(Scene& scene, f64 timeStep){
        EV_profile_function();
        static std::vector<aabb2d_t> collider2dBounds(16);

        tree2d.clear();
        contacts2d.clear();
        collider2dBounds.clear();

        registry_t& registry = scene._registry;

        auto bc2d = registry.view<boxCollider2d_c>();
        auto cc2d = registry.view<circleCollider2d_c>();

        for(entt::entity ent : bc2d){
            boxCollider2d_c& comp = bc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            collider2dBounds.push_back(cld->getBounds());

            auto bb = collider2dBounds.back(); 
            tree2d.insert(cld, collider2dBounds.back());
        }

        for(entt::entity ent : cc2d){
            circleCollider2d_c& comp = cc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            collider2dBounds.push_back(cld->getBounds());
            tree2d.insert(cld, collider2dBounds.back());
        }

        for(auto bounds : collider2dBounds){
            static std::vector<ref<collider2d_c>> candidates(16);
            candidates.clear();

            tree2d.query(bounds, candidates);
            if(candidates.size() < 2) continue;
            for(int i=0; i<candidates.size(); i++){
                for(int j=i+1; j<candidates.size(); j++){
                    bool col = CollisionDetector2D::checkForContacts(candidates[i], candidates[j], contacts2d);
                    if(col){
                        body_contact2d_t& cd = contacts2d.back();

                        collision2d_t col{
                            .self = candidates[i]->entity,
                            .other = candidates[j]->entity,
                            .contact = cd.relativeContactPointA + vec2{cd.transformA->position},
                            .normal = cd.contactNormal,
                            .penetration = cd.penetration
                        };

                        collision2dResults.push_back(col);
                        if( (cd.rigidbody2dA == nullptr || cd.rigidbody2dA->definition & BodyDefBits::Static) &&
                            (cd.rigidbody2dB == nullptr || cd.rigidbody2dB->definition & BodyDefBits::Static) )
                            contacts2d.pop_back();
                    }
                }
            }
        }
    }

    void PhysicsHandler::simulate2d(Scene& scene, f64 timeStep){
        EV_profile_function();
        registry_t& registry = scene._registry;
        auto spjgrp = registry.group<springJoint2d_c>(entt::get<transform_c, rigidbody2d_c>);
        auto phygrp = registry.group<rigidbody2d_c>(entt::get<transform_c>);

        for(auto ent:spjgrp){
            const auto& [spr, tfr, rb] = spjgrp.get(ent);
            spr.updateForce(tfr, rb);
        }

        for(auto ent:phygrp){
            const auto& [rb2d, tfr] = phygrp.get(ent);
            if(rb2d.definition & BodyDefBits::UseGravity) rb2d.addForce(_gravity, ForceMode::Acceleration);
            rb2d.integrate(tfr, timeStep);
        }

        collision2dResults.clear();
        generateContacts2d(scene, timeStep);
        _contactResolver2d.resolveContacts(contacts2d);

        for(auto& res : collision2dResults){
            if(res.self.isValid() && res.self.has<EvScript>()){
                res.self.get<EvScript>().collisionCallback(res);
            }

            if(res.other.isValid() && res.other.has<EvScript>()){
                std::swap(res.self, res.other);
                res.self.get<EvScript>().collisionCallback(res);
            }
        }
    }
}
