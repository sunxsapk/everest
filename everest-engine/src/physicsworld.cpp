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

    void PhysicsHandler::simulate(Scene* scene, f64 timeStep){
        simulate2d(scene, timeStep);
        simulate3d(scene, timeStep);
    }

    //TODO: I am here
    std::vector<BodyContact> PhysicsHandler::generateContacts(Scene* scene, f64 timeStep){
        std::vector<BodyContact> contacts;
        return  contacts;
    }

    std::vector<BodyContact2D> PhysicsHandler::generateContacts2d(Scene* scene, f64 timeStep){
        std::vector<BodyContact2D> contacts;
        return  contacts;
    }

    void PhysicsHandler::simulate2d(Scene* scene, f64 timeStep){
        auto sprgrp = scene->_registry.group<springJoint2d_c>(entt::get<transform_c, rigidbody2d_c>);
        auto phygrp = scene->_registry.group<rigidbody2d_c>(entt::get<transform_c>);

        for(auto ent:sprgrp){
            const auto& [spr, tfr, rb] = sprgrp.get(ent);
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

    void PhysicsHandler::simulate3d(Scene* scene, f64 timeStep){
        auto sprgrp = scene->_registry.group<springJoint_c>(entt::get<transform_c, rigidbody_c>);
        auto phygrp = scene->_registry.group<rigidbody_c>(entt::get<transform_c>);

        for(auto ent:sprgrp){
            const auto& [spr, tfr, rb] = sprgrp.get(ent);
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
