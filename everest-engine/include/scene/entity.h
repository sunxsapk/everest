/*
 * ============= Entity ================
 * Author: Sunil Sapkota
 * Description: Entity is a unique object in a scene that is composed of different components
 */

#pragma once
#include "entt.hpp"
#include "scene.h"
#include "core/dbghelper.h"

namespace Everest {

    class Entity {
        public:
        Entity() = default;
        Entity(entt::entity id, Scene* scene):_id(id), _scene(scene){}

        operator entt::entity() const {return _id;}
        operator u32() const {return (u32)_id;}
        bool operator ==(Entity& other) const {return _id == other._id && _scene == other._scene;}
        bool operator !=(Entity& other) const {return !(*this == other);}

        inline bool isValid() const {return _id != entt::null && _scene->_registry.valid(_id);}

        template<typename... component_t>
        inline bool has(){
            return _id != entt::null && _scene->_registry.all_of<component_t...>(_id);
        }

        template<typename component_t, typename... args_t>
        inline component_t& add(args_t&&... args){
            ASSERT(!has<component_t>(), "Entity already has the component");
            component_t& comp = _scene->_registry.emplace<component_t>(_id, *this, std::forward<args_t>(args)...);
            _scene->onComponentAdded(*this, comp);
            return comp;
        }

        template<typename component_t, typename... args_t>
        inline component_t& tryAdd(args_t&&... args){
            if(has<component_t>()) return get<component_t>();
            return add<component_t>(std::forward<args_t>(*this, args)...);
        }

        template<typename component_t>
        inline component_t& get(){
            ASSERT(has<component_t>(), "Entity does not have the component");
            return _scene->_registry.get<component_t>(_id);
        }

        template<typename component_t>
        inline void remove(){
            _scene->onComponentRemoved(*this, get<component_t>());
            _scene->_registry.remove<component_t>(_id);
        }

        template<typename component_t>
        inline void tryRemove(){
            if(has<component_t>()) remove<component_t>();
        }

        inline void destroy(){
            _scene->destroyEntity(*this);
        }

        inline Scene* getScene(){
            return _scene;
        }

        private:
        entt::entity _id{entt::null};
        Scene* _scene = nullptr;
        friend class Scene;
    };
}
