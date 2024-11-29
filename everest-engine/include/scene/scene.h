/*
 * =============== Scene =====================
 * Author: Sunil Sapkota
 * Description: Scene is the handler for the game world. It manages things like: Entity-tree, ECS,
 * sequence of game events, etc.
 */
#pragma once

#include <entt.hpp>
#include "math/types.h"

namespace Everest {

    struct Entity;
    class Scene {
        public:
            Scene();
            ~Scene();

            Entity createEntity(const char * name = "Entity");
            void destroyEntity(Entity& entity);

            void onUpdate();
            void onViewportResize(uvec2 viewportSize);

        private:
            entt::registry _registry;
            uvec2 _viewportSize;
            friend class Entity;
            friend class SceneHeirarchyUI;
            friend class SceneSerializer;
    };
}
