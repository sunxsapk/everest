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

    struct scriptHandler_t {
        std::filesystem::path scriptpath;
        luastate_t state;

        std::function<void()> onCreate;
        std::function<void(double)> onUpdate;
        std::function<void(collision2d_t)> onCollision;

        bool _initialized = false;

        template<typename Alloc>
        scriptHandler_t(Alloc& alloc){ }
        scriptHandler_t(){};
        scriptHandler_t(const scriptHandler_t& other);
        scriptHandler_t& operator=(const scriptHandler_t& other);

        void init(Entity entity);
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        inline bool getSerializedFields(sol::table& resultTable){
            if(state["__serialize"].valid()){
                resultTable = state["__serialize"];
                return true;
            }
            return false;
        }

        void setScriptPath(std::filesystem::path path, Entity ent);
        const char* getScriptName();
    };

    struct evscript_c {
        Entity entity;
        std::vector<scriptHandler_t> scripts;

        template<typename Alloc>
        evscript_c(Alloc& alloc){ init();}
        evscript_c(Entity entity_):entity(entity_){ }
        evscript_c(const evscript_c& other);
        evscript_c& operator=(const evscript_c& other);

        void init();
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        friend class Scene;
    };

}
}
