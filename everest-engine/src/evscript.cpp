#include "scripting/evscript.h"
#include "utils/assetsManager.h"

namespace Everest {
namespace Scripting {

    scriptHandler_t::scriptHandler_t(const scriptHandler_t& other){
        *this = other;
    }

    scriptHandler_t::scriptHandler_t(Entity ent, std::filesystem::path path)
    :scriptpath(path){
        init(ent);
    }

    scriptHandler_t& scriptHandler_t::operator=(const scriptHandler_t& other){
        if(this == &other) return *this;
        scriptpath = other.scriptpath;
        state = other.state;
        onCreate = other.onCreate;
        onUpdate = other.onUpdate;
        onCollision = other.onCollision;
        _initialized = false;
        return *this;
    }

    void scriptHandler_t::init(Entity entity){
        if(!entity.isValid()) return;
        if( !scriptpath.has_extension() ||
            AssetsManager::getAssetsType(scriptpath) != AssetsType::SCRIPT
        ) return;

        state = createRef<luastate_t>();
        auto& __state = *state;
        __state.open_libraries(lualibs::base, lualibs::math);
        registerTypes(__state);
        auto lres = __state.load_file(scriptpath.c_str());
        if(!lres.valid()){
            throw std::invalid_argument("Unable to load lua script");
        }
        lres();
        __state["entity"] = entity;
        if(__state["OnCreate"].valid()) {
            onCreate = __state["OnCreate"];
        }
        if(__state["OnUpdate"].valid()){
            onUpdate = __state["OnUpdate"];
        }
        if(__state["OnCollision"].valid()){
            onCollision = __state["OnCollision"];
        }
        EVLog_Msg("Parsed script %s with serialization validity(%d)", scriptpath.c_str(), __state["__serialize"].valid());
    }

    bool scriptHandler_t::getSerializedFields(sol::table& resultTable) {
        if(state == nullptr || (*state)["__serialize"].valid()){
            resultTable = (*state)["__serialize"];
            return true;
        }
        return false;
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
        entity = other.entity;
        scripts = other.scripts;
        return *this;
    }

    void evscript_c::makeCopyUsing(const evscript_c& other, Entity entity_){
        entity = entity_;
        this->scripts.clear();
        for(auto& script : other.scripts){
            this->scripts.push_back(scriptHandler_t());
            auto& lastScript = this->scripts.back();
            lastScript.scriptpath = script.scriptpath;
            lastScript.init(entity);

            auto& _sstate = *script.state;
            auto& _lstate = *lastScript.state;
            if(!_sstate["__serialize"].valid()) continue;
            sol::table st = _sstate["__serialize"];
            for(auto& [k, v] : st){
                if(!_sstate[k].valid() || !_lstate[k].valid()) continue;
                _lstate[k] = _sstate[k];
            }
        }
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

    void evscript_c::addScript(const std::filesystem::path& path){
        for(auto& script : scripts){
            if(script.scriptpath == path) return;
        }
        scripts.push_back(scriptHandler_t(entity, path));
    }

}
}
