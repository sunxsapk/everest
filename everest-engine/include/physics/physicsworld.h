/*
 * ========== Physics World =============
 * Author: Sunil Sapkota
 * Description: Physics world is the handler for all 2d and 3d physics bodies in the scene. It
 * simulates the physics on all of the objects inside the world.
 */



#pragma once


#include "math/types.h"
#include "physics/collisionResolver.h"

namespace Everest {

    using registry_t = entt::registry;

    class PhysicsHandler {
        public:

            static void init(u32 simulationSteps = 4, vec3 gravity = {0.f, -9.8f, 0.f});
            static void quit();

            static void simulate(Scene& scene, f64 timeStep);

        private:
            static u32 _simulationSteps;
            static vec3 _gravity;
            static ContactResolver _contactResolver;
            static ContactResolver2D _contactResolver2d;

            static void simulate2d(Scene& scene, f64 timeStep);
            static void simulate3d(Scene& scene, f64 timeStep);
            static std::vector<BodyContact> generateContacts(Scene& scene, f64 timeStep);
            static std::vector<BodyContact2D> generateContacts2d(Scene& scene, f64 timeStep);
    };
}
