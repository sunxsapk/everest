#include "physics/physicsworld.h"
#include "scene/components.h"



namespace Everest {

    u32 PhysicsHandler::_simulationSteps = 4;
    vec3 PhysicsHandler::_gravity = vec3(0.f, -9.8f, 0.f);

    void PhysicsHandler::init(u32 simulationSteps, vec3 gravity) {
        _simulationSteps = simulationSteps;
        _gravity = gravity;
    }

    void PhysicsHandler::quit(){ }

    void PhysicsHandler::simulate(Scene* scene, f64 timeStep){
        f32 dt = timeStep / _simulationSteps;
        simulate2d(scene, dt);
        simulate3d(scene, dt);
    }

    void PhysicsHandler::simulate2d(Scene* scene, f64 timeStep){
        auto sprgrp = scene->_registry.group<springJoint2d_c>(entt::get<transform_c, rigidbody2d_c>);
        auto phygrp = scene->_registry.group<rigidbody2d_c>(entt::get<transform_c>);

        for(int i=0; i<_simulationSteps; i++){
            for(auto ent:sprgrp){
                const auto& [spr, tfr, rb] = sprgrp.get(ent);
                spr.updateForce(tfr.position, rb);
            }

            for(auto ent:phygrp){
                const auto& [rb2d, tfr] = phygrp.get(ent);
                if(rb2d.useGravity) rb2d.addForce(_gravity);
                vec2 dp = rb2d.integrate(timeStep);
                tfr.position += vec3(dp, 0);
            }
        }
    }

    void PhysicsHandler::simulate3d(Scene* scene, f64 timeStep){
        auto sprgrp = scene->_registry.group<springJoint_c>(entt::get<transform_c, rigidbody_c>);
        auto phygrp = scene->_registry.group<rigidbody_c>(entt::get<transform_c>);

        for(int i=0; i<_simulationSteps; i++){
            for(auto ent:sprgrp){
                const auto& [spr, tfr, rb] = sprgrp.get(ent);
                spr.updateForce(tfr.position, rb);
            }

            for(auto ent:phygrp){
                const auto& [rb, tfr] = phygrp.get(ent);
                if(rb.useGravity) rb.addForce(_gravity);
                tfr.position += rb.integrate(timeStep);
            }
        }
    }

}
