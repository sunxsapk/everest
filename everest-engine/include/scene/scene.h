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
#include "utils/uuid.h"

namespace Everest {

    struct camera_c;
    class Entity;

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
            void onViewportResize(uvec2 viewportOffset, uvec2 viewportSize);

            camera_c* setMainCamera(Entity& entity);
            camera_c* getMainCamera();
            Entity getMainCameraEntity();

            vec2 worldToScreen(vec3 worldPos);
            vec3 screenToWorld(vec2 screenPos);
            vec3 screenToWorldDir(vec2 screenPos);

            Entity getEntityFromId(UUID id);

            static ref<Scene> copy(ref<Scene>& other);

        private:
            entt::entity mainCamera;
            std::map<UUID, entt::entity> entityDB;
            entt::registry _registry;
            uvec2 _viewportOffset, _viewportSize; // TODO: add event listener for viewport resize during runtime
            std::string _name;

            void fetchTargetCamera();
            void onDestroyEntity(Entity& ent);

            template<typename T>
            inline void onComponentAdded(Entity& entity, T& component){}

            template<typename T>
            inline void onComponentRemoved(Entity& entity, T& component){}
            void onComponentRemoved(Entity& entity, camera_c& component);

            friend class Entity;
            friend class SceneHeirarchyUI;
            friend class SceneSerializer;
            friend class PhysicsHandler;
    };
}
