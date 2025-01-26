#include "physics/physicsworld.h"
#include "scene/components.h"



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

    void PhysicsHandler::simulate(registry_t& registry, f64 timeStep){
        simulate2d(registry, timeStep);
        simulate3d(registry, timeStep);
    }

    std::vector<BodyContact> PhysicsHandler::generateContacts(registry_t& registry, f64 timeStep){
        std::vector<BodyContact> contacts;
        return  contacts;
    }

    std::vector<BodyContact2D> PhysicsHandler::generateContacts2d(registry_t& registry, f64 timeStep){
        std::vector<BodyContact2D> contacts;
       //TODO: I am here

        return  contacts;
    }

    void PhysicsHandler::simulate2d(registry_t& registry, f64 timeStep){
        auto sprgrp = registry.group<springJoint2d_c>(entt::get<transform_c, rigidbody2d_c>);
        auto phygrp = registry.group<rigidbody2d_c>(entt::get<transform_c>);

        for(auto ent:sprgrp){
            const auto& [spr, tfr, rb] = sprgrp.get(ent);
            spr.updateForce(tfr, rb);
        }

        for(auto ent:phygrp){
            const auto& [rb2d, tfr] = phygrp.get(ent);
            if(rb2d.useGravity) rb2d.addForce(_gravity, ForceMode::Acceleration);
            rb2d.integrate(tfr, timeStep);
        }

        std::vector<BodyContact2D> contacts = generateContacts2d(registry, timeStep);
        _contactResolver2d.resolveContacts(contacts, timeStep);
    }

    void PhysicsHandler::simulate3d(registry_t& registry, f64 timeStep){
        auto sprgrp = registry.group<springJoint_c>(entt::get<transform_c, rigidbody_c>);
        auto phygrp = registry.group<rigidbody_c>(entt::get<transform_c>);

        for(auto ent:sprgrp){
            const auto& [spr, tfr, rb] = sprgrp.get(ent);
            spr.updateForce(tfr, rb);
        }

        for(auto ent:phygrp){
            const auto& [rb, tfr] = phygrp.get(ent);
            if(rb.useGravity) rb.addForce(_gravity, ForceMode::Acceleration);
            rb.integrate(tfr, timeStep);
        }

        std::vector<BodyContact> contacts = generateContacts(registry, timeStep);
        _contactResolver.resolveContacts(contacts, timeStep);
    }

}
