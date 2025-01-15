/*
 * =============== Scene =====================
 * Author: Sunil Sapkota
 * Description: Scene is the handler for the game world. It manages things like: Entity-tree, ECS,
 * sequence of game events, etc.
 */
#pragma once

#include <entt.hpp>
#include "core/utils.h"
#include "math/types.h"
#include "scene/camera.h"
#include "utils/uuid.h"

namespace Everest {

    struct Entity;
    class Scene {
        public:
            Scene() = default;
            Scene(const char* name);
            Scene(std::string name);
            ~Scene();

            Entity createEntity(const char * name = "Entity");
            Entity createEntityUUID(UUID id, const char * name = "Entity");
            void destroyEntity(Entity& entity);

            void onRender();
            void onEditorRender(camera_c& camera, mat4 transform);
            void onScenePlay();
            void onSceneStop();
            void onUpdate();
            void onViewportResize(uvec2 viewportSize);

            static ref<Scene> copy(ref<Scene>& other);

        private:
            entt::registry _registry;
            uvec2 _viewportSize;
            std::string _name;
        private:
            friend class Entity;
            friend class SceneHeirarchyUI;
            friend class SceneSerializer;
            friend class PhysicsHandler;
    };
}
