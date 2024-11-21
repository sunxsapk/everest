/*
 * =============== Scene =====================
 * Author: Sunil Sapkota
 * Description: Scene is the handler for the game world. It manages things like: Entity-tree, ECS,
 * sequence of game events, etc.
 */
#pragma once

#include <entt.hpp>

namespace Everest {

    struct Entity;
    class Scene {
        public:
            Scene();
            ~Scene();

            Entity createEntity(const char * name = "Entity");

            void onUpdate();
            void onViewportResize(uvec2 viewportSize);

        private:
            entt::registry _registry;
            uvec2 _viewportSize;
            friend class Entity;
    };
}
