/*
 * ============ Scriptable ================
 * Author: Sunil Sapkota
 * Description: Scriptable is a blueprint for C++ script component that can be added and used
 * by components. It has some predefined functions which should be defined by each scripts.
 */
#pragma once


#include "entity.h"
#include "scene/component_type.h"

namespace Everest {
    class Scriptable {
        public:
            virtual ~Scriptable(){}

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

        protected:
            virtual void onCreate(){}
            virtual void onUpdate(f32 deltaTime){}
            virtual void onDestroy(){}
        private:
            Entity _entity;
            friend class Scene;
    };

    struct nativeScript_c : public component_t {
        Scriptable* _instance;

        Scriptable* (*create)();
        void (*destroy)(nativeScript_c*);

        nativeScript_c() = default;
        nativeScript_c(Entity ent):component_t(ent){}

        template<typename script_t>
        void bind(){
            create = [](){return static_cast<Scriptable*>(new script_t());};
            destroy = [](nativeScript_c* nsc){
                delete nsc->_instance;
                nsc->_instance = nullptr;
            };
        }
    };

}
