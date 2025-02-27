/*
 * ========== Physics World =============
 * Author: Sunil Sapkota
 * Description: Physics world is the handler for all 2d and 3d physics bodies in the scene. It
 * simulates the physics on all of the objects inside the world.
 */



#pragma once


#include "math/types.h"
#include "physics/collisionResolver.h"

#include "physics/phyconfig.h"

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
            static std::vector<body_contact2d_t> contacts2d;
            static contact2d_resolver_t _contactResolver2d;
            static aabb2d_tree_t<collider2d_c> tree2d;

            static void simulate2d(Scene& scene, f64 timeStep);
            static void generateContacts2d(Scene& scene, f64 timeStep);

#ifndef __NO_3D__
            static std::vector<body_contact_t> contacts;
            static contact_resolver_t _contactResolver3d;
            static aabb_tree_t<collider_c> tree3d;

            static void simulate3d(Scene& scene, f64 timeStep);
            static void generateContacts(Scene& scene, f64 timeStep);
#endif
    };
}
