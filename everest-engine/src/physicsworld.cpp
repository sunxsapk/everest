#include "physics/physicsworld.h"
#include "physics/aabbtree2d.h"
#include "scene/components.h"
#include "physics/collisionDetector.h"



namespace Everest {

    u32 PhysicsHandler::_simulationSteps = 4;
    vec3 PhysicsHandler::_gravity = vec3(0.f, -9.8f, 0.f);
    ContactResolver PhysicsHandler::_contactResolver;
    ContactResolver2D PhysicsHandler::_contactResolver2d;

    void PhysicsHandler::init(u32 simulationSteps, vec3 gravity) {
        _simulationSteps = simulationSteps;
        _gravity = gravity;
    }

    void PhysicsHandler::quit(){ }

    void PhysicsHandler::simulate(Scene& scene, f64 timeStep){
        simulate2d(scene, timeStep);
        simulate3d(scene, timeStep);
    }

    std::vector<BodyContact> PhysicsHandler::generateContacts(Scene& scene, f64 timeStep){
        std::vector<BodyContact> contacts;
        // TODO: 3d collision detection and resolution
        return  contacts;
    }

    std::vector<BodyContact2D> PhysicsHandler::generateContacts2d(Scene& scene, f64 timeStep){
        registry_t& registry = scene._registry;

        auto bc2d = registry.view<boxCollider2d_c>();
        auto cc2d = registry.view<circleCollider2d_c>();
        size_t ccount = bc2d->size() + cc2d.size();

        AABBTree2D<collider2d_c> tree;
        std::vector<AABB2D> colliderBounds(ccount); 

        EVLog_Msg("\n==== creating tree for %d colliders", ccount);
        for(entt::entity ent : bc2d){
            boxCollider2d_c& comp = bc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            colliderBounds.push_back(cld->getBounds());
            tree.insert(cld, colliderBounds.back());
        }

        for(entt::entity ent : cc2d){
            circleCollider2d_c& comp = cc2d->get(ent);
            ref<collider2d_c> cld = createRef<collider2d_c>(comp, Entity{ent, &scene});
            colliderBounds.push_back(cld->getBounds());
            tree.insert(cld, colliderBounds.back());
        }

        EVLog_Msg("\n==== contacts check");
        std::vector<BodyContact2D> contacts;

        for(auto bounds : colliderBounds){
            std::vector<ref<collider2d_c>> candidates;
            tree.query(bounds, candidates);

            if(candidates.size() < 2) continue;
            EVLog_Msg("==== Contacts candidates size : %d", candidates.size());

            for(int i=0; i<candidates.size(); i++){
                for(int j=i+1; j<candidates.size(); j++){
                    if(CollisionDetector2D::checkForContacts(candidates[i], candidates[j], contacts)){
                        EVLog_Wrn("colliding: %u %u", (u32)candidates[i]->entity, (u32)candidates[j]->entity);
                    }
                }
            }
        }

        return  contacts;
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

        std::vector<BodyContact2D> contacts = generateContacts2d(scene, timeStep);
        _contactResolver2d.resolveContacts(contacts, timeStep);
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

        std::vector<BodyContact> contacts = generateContacts(scene, timeStep);
        _contactResolver.resolveContacts(contacts, timeStep);
    }

}
