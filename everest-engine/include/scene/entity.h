/*
 * ============= Entity ================
 * Author: Sunil Sapkota
 * Description: Entity is a unique object in a scene that is composed of different components
 */

#pragma once
#include "entt.hpp"
#include "scene.h"

namespace Everest {

    class Entity {
        public:
        Entity() = default;
        Entity(entt::entity id, Scene* scene):_id(id), _scene(scene){}

        template<typename... component_t>
        inline bool has(){
            return _scene->_registry.all_of<component_t...>(_id);
        }

        template<typename component_t, typename... args_t>
        inline component_t& add(args_t&&... args){
            ASSERT(!has<component_t>(), "Entity already has the component");
            return _scene->_registry.emplace<component_t>(_id, std::forward<args_t>(args)...);
        }

        template<typename component_t>
        inline component_t& get(){
            ASSERT(has<component_t>(), "Entity does not have the component");
            return _scene->_registry.get<component_t>(_id);
        }

        template<typename component_t>
        inline void remove(){
            _scene->_registry.remove<component_t>(_id);
        }

        private:
        entt::entity _id{entt::null};
        Scene* _scene = nullptr;
    };
}
