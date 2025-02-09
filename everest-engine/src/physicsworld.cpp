#include "physics/physicsworld.h"
#include "physics/aabbtree2d.h"
#include "scene/components.h"
#include "physics/collisionDetector.h"



namespace Everest {

    u32 PhysicsHandler::_simulationSteps = 4;
    vec3 PhysicsHandler::_gravity = vec3(0.f, -9.8f, 0.f);
    ContactResolver PhysicsHandler::_contactResolver;
    ContactResolver2D PhysicsHandler::_contactResolver2d;
    std::vector<BodyContact2D> PhysicsHandler::contacts2d(16);
    std::vector<BodyContact> PhysicsHandler::contacts(16);

    void PhysicsHandler::init(u32 simulationSteps, vec3 gravity) {
        _simulationSteps = simulationSteps;
        _gravity = gravity;
    }

    void PhysicsHandler::quit(){ }

    void PhysicsHandler::simulate(Scene& scene, f64 timeStep){
        simulate2d(scene, timeStep);
        simulate3d(scene, timeStep);
    }

    void PhysicsHandler::generateContacts(Scene& scene, f64 timeStep){
        // TODO: 3d collision detection and resolution
        contacts.clear();
    }

    void PhysicsHandler::generateContacts2d(Scene& scene, f64 timeStep){
        contacts2d.clear();
        registry_t& registry = scene._registry;

        auto bc2d = registry.view<boxCollider2d_c>();
        auto cc2d = registry.view<circleCollider2d_c>();
        size_t ccount = bc2d->size() + cc2d.size();

        AABBTree2D<collider2d_c> tree;
        std::vector<AABB2D> colliderBounds(ccount); 

        for(entt::entity ent : bc2d){
            boxCollider2d_c& comp = bc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            colliderBounds.push_back(cld->getBounds());

            auto bb = colliderBounds.back(); 
            tree.insert(cld, colliderBounds.back());
        }

        for(entt::entity ent : cc2d){
            circleCollider2d_c& comp = cc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            colliderBounds.push_back(cld->getBounds());
            tree.insert(cld, colliderBounds.back());
        }

        for(auto bounds : colliderBounds){
            std::vector<ref<collider2d_c>> candidates;
            tree.query(bounds, candidates);

            if(candidates.size() < 2) continue;

            for(int i=0; i<candidates.size(); i++){
                for(int j=i+1; j<candidates.size(); j++){
                    CollisionDetector2D::checkForContacts(candidates[i], candidates[j], contacts2d);
                }
            }
        }
    }

    void PhysicsHandler::simulate2d(Scene& scene, f64 timeStep){
        registry_t& registry = scene._registry;
        auto spjgrp = registry.group<springJoint2d_c>(entt::get<transform_c, rigidbody2d_c>);
        auto phygrp = registry.group<rigidbody2d_c>(entt::get<transform_c>);

        for(auto ent:spjgrp){
            const auto& [spr, tfr, rb] = spjgrp.get(ent);
            spr.updateForce(tfr, rb);
        }

        for(auto ent:phygrp){
            const auto& [rb2d, tfr] = phygrp.get(ent);
            if(rb2d.useGravity) rb2d.addForce(_gravity, ForceMode::Acceleration);
            rb2d.integrate(tfr, timeStep);
        }

        generateContacts2d(scene, timeStep);
        _contactResolver2d.resolveContacts(contacts2d, timeStep);
    }

    void PhysicsHandler::simulate3d(Scene& scene, f64 timeStep){
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
        _contactResolver.resolveContacts(contacts, timeStep);
    }

}
