/*
 * ============ Scriptable ================
 * Author: Sunil Sapkota
 * Description: Scriptable is a blueprint for C++ script component that can be added and used
 * by components. It has some predefined functions which should be defined by each scripts.
 */
#pragma once


#include "entity.h"

namespace Everest {
    class Scriptable {
        public:
            template<typename t>
            inline t& get(){
                return _entity.get<t>();
            }

            template<typename... t>
            inline bool has(){
                return _entity.has<t...>();
            }

            template<typename component_t, typename... args_t>
            inline component_t& add(args_t&&... args){
                return _entity.add<component_t>(std::forward<args_t>(args)...);
            }

            template<typename component_t>
            inline void remove(){
                _entity.remove<component_t>();
            }

        private:
            Entity _entity;
    };
}
