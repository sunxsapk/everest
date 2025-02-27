/*
 * ======== EvScript ===========
 * Author: Sunil Sapkota
 * Description : EvScript or Everest Script is the component which handles the runtime lua script.
 * It provides a way for user to use and modify the script at runtime. Alongside all the features
 * provided by the engine, writing a custom behaviour for entities in lua is a good addition to the
 * engine.
 */

#pragma once

#include "physics/collision.h"
#include "scene/entity.h"
#include "scripting/typeregistry.h"
#include <filesystem>
#include <functional>

namespace Everest {
namespace Scripting {

    struct evscript_c {
        std::filesystem::path scriptpath;
        Entity _entity;

        evscript_c();
        evscript_c(Entity entity);
        evscript_c(const evscript_c& other);
        evscript_c& operator=(const evscript_c& other);

        void init();
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        inline void setScriptPath(std::filesystem::path path){
            if(scriptpath == path) return;
            scriptpath = path;
            init();
        }

        inline const char* getScriptName(){
            return scriptpath.stem().c_str();
        }

        template<typename Alloc>
        evscript_c(Alloc& alloc){
            init();
        }

        private:
        luastate_t state;
        bool _initialized = false;

        std::function<void()> onCreate;
        std::function<void(double)> onUpdate;
        std::function<void(collision2d_t)> onCollision;

        friend class Scene;
    };

}
}
