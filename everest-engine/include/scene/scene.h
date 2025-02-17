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

    struct transform_c;
    struct Entity;
    class Scene {
        public:
            Scene() = default;
            Scene(const char* name);
            Scene(std::string name);
            ~Scene();

            Entity createEntity(const char * name = "Entity");
            Entity createEntityUUID(UUID id, const char * name = "Entity");
            Entity duplicateEntity(Entity& entity);
            void destroyEntity(Entity& entity);

            void onRender();
            void onEditorBeginRender(camera_c& camera, mat4 transform);
            void onEditorRender(bool renderPhysicsShapes = false);
            void onEditorEndRender();
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
            struct RendererCamera {
                camera_c* camera = nullptr;
                transform_c* transform = nullptr;
            } mainCamera;

            void fetchTargetCamera();

            friend class Entity;
            friend class SceneHeirarchyUI;
            friend class SceneSerializer;
            friend class PhysicsHandler;
    };
}
