/*
 * =============== Scene =====================
 * Author: Sunil Sapkota
 * Description: Scene is the handler for the game world. It manages things like: Entity-tree, ECS,
 * sequence of game events, etc.
 */
#pragma once

#include <entt.hpp>
#include "entity.h"

namespace Everest {

    struct Entity;
    class Scene {
        public:
            Scene();
            ~Scene();

            Entity createEntity(const char * name = "Entity");

            void onUpdate();

        private:
            entt::registry _registry;
            friend class Entity;
    };
}
