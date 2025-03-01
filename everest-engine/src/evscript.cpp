#include "scripting/evscript.h"
#include "utils/assetsManager.h"

namespace Everest {
namespace Scripting {

    scriptHandler_t::scriptHandler_t(const scriptHandler_t& other){
        *this = other;
    }

    scriptHandler_t& scriptHandler_t::operator=(const scriptHandler_t& other){
        if(this == &other) return *this;
        scriptpath = other.scriptpath;
        onUpdate = nullptr;
        onCreate = nullptr;
        onCollision = nullptr;
        _initialized = false;
        return *this;
    }

    void scriptHandler_t::init(Entity entity){
        if(!entity.isValid()) return;
        if( !scriptpath.has_extension() ||
            AssetsManager::getAssetsType(scriptpath) != AssetsType::SCRIPT
        ) return;

        state = luastate_t();
        state.open_libraries(lualibs::base, lualibs::math);
        registerTypes(state);
        auto lres = state.load_file(scriptpath.c_str());
        if(!lres.valid()){
            return;
            // throw std::invalid_argument("Unable to load lua script");
        }
        lres();
        state["entity"] = entity;
        if(state["OnCreate"].valid()) {
            onCreate = state["OnCreate"];
        }
        EVLog_Msg("ch1");
        if(state["OnUpdate"].valid()){
            onUpdate = state["OnUpdate"];
        }
        EVLog_Msg("ch1");
        if(state["OnCollision"].valid()){
            onCollision = state["OnCollision"];
        }
        EVLog_Msg("ch1");
        EVLog_Msg("Parsed script");
    }

    void scriptHandler_t::update(double deltaTime){
        if(!_initialized){
            _initialized = true;
            EVLog_Msg("Initialized script %s", scriptpath.c_str());
            if(onCreate) {
                onCreate();
            }
        }

        if(onUpdate){
            onUpdate(deltaTime);
        }
    }

    void scriptHandler_t::collisionCallback(collision2d_t& data){
        if(onCollision){
            onCollision(data);
        }
    }

    void scriptHandler_t::setScriptPath(std::filesystem::path path, Entity ent){
        if(scriptpath == path) return;
        scriptpath = path;
        init(ent);
    }

    const char* scriptHandler_t::getScriptName(){
        return scriptpath.stem().c_str();
    }

    evscript_c::evscript_c(const evscript_c& other){
        *this = other;
    }

    evscript_c& evscript_c::operator=(const evscript_c& other){
        if(this == &other) return *this;
        this->scripts.clear();
        for(auto& script : other.scripts){
            auto nscr = script;
            this->scripts.push_back(nscr);
        }
        return *this;
    }

    void evscript_c::init() {
        for(auto& script : scripts){
            script.init(entity);
        }
    }

    void evscript_c::update(double deltaTime){
        for(auto& script : scripts){
            script.update(deltaTime);
        }
    }

    void evscript_c::collisionCallback(collision2d_t& data){
        for(auto& script : scripts){
            script.collisionCallback(data);
        }
    }

}
}
