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
#include "scene/component_type.h"
#include "scene/entity.h"
#include "extended_types.h"
#include <filesystem>
#include <functional>

namespace Everest {
namespace Scripting {

    struct scriptHandler_t {
        std::filesystem::path scriptpath;
        ref<luastate_t> state = nullptr;

        std::function<void()> onCreate;
        std::function<void(double)> onUpdate;
        std::function<void(collision2d_t)> onCollision;

        bool _initialized = false;

        template<typename Alloc>
        scriptHandler_t(Alloc& alloc){ }
        scriptHandler_t(){};
        scriptHandler_t(Entity ent, std::filesystem::path path);
        scriptHandler_t(const scriptHandler_t& other);
        scriptHandler_t& operator=(const scriptHandler_t& other);

        void init(Entity entity);
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        bool getSerializedFields(sol::table& resultTable) const;

        void setScriptPath(std::filesystem::path path, Entity ent);
        std::string getScriptName();

        sol::table call(std::string func_name, sol::table inp_args);
        template<typename T>
        T get(std::string var_name){
            return (*state)[var_name];
        }
    };

    struct evscript_c : public component_t {
        std::vector<scriptHandler_t> scripts;

        evscript_c() = default;
        template<typename Alloc>
        evscript_c(Alloc& alloc) {scripts.reserve(8);} //make way for 8 scripts by default
        evscript_c(Entity entity_):component_t(entity_){ scripts.reserve(8);}
        evscript_c(const evscript_c& other);
        evscript_c& operator=(const evscript_c& other);

        void makeCopyUsing(const evscript_c& other, Entity entity);
        void init();
        void update(double deltaTime);
        void collisionCallback(collision2d_t& data);

        scriptHandler_t& addScript(const std::filesystem::path& path);

        scriptHandler_t* tryGetScriptHandler(std::string name);

        friend class Scene;
    };

}
}
