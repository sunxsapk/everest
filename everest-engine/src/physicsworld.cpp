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
        auto phygrp = scene->_registry.group<rigidbody2d_c>(entt::get<transform_c>);
        
        f32 dt = timeStep / _simulationSteps;
        for(int i=0; i<_simulationSteps; i++){

            for(auto ent:phygrp){
                const auto& [rb2d, tfr] = phygrp.get(ent);

                rb2d.addForce(_gravity);
                vec2 dp = rb2d.integrate(dt);
                tfr.position += vec3(dp, 0);
            }

        }
    }

}
